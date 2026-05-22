param(
    [string]$RepoRoot = (Resolve-Path "$PSScriptRoot\..\..\..").Path,
    [ValidateSet("Markdown", "Json")]
    [string]$Format = "Markdown",
    [string]$OutputPath = ""
)

$ErrorActionPreference = "Stop"

$cmakePath = Join-Path $RepoRoot "CMakeLists.txt"
if (-not (Test-Path -LiteralPath $cmakePath)) {
    throw "CMakeLists.txt not found under RepoRoot: $RepoRoot"
}

$cmakeLines = Get-Content -LiteralPath $cmakePath

$sets = @{}
$currentName = $null
$currentItems = @()

foreach ($line in $cmakeLines) {
    if ($null -eq $currentName) {
        if ($line -match '^\s*set\(\s*([A-Za-z0-9_]+)\s*(.*)$') {
            $currentName = $matches[1]
            $currentItems = @()
            $rest = $matches[2].Trim()
            if ($rest -eq ')') {
                $sets[$currentName] = @()
                $currentName = $null
            } elseif ($rest.Length -gt 0) {
                if ($rest.EndsWith(')')) {
                    $item = $rest.Substring(0, $rest.Length - 1).Trim()
                    $sets[$currentName] = if ($item.Length -gt 0) { @($item.Trim('"')) } else { @() }
                    $currentName = $null
                } else {
                    $currentItems += $rest.Trim('"')
                }
            }
        }
        continue
    }

    if ($line -match '^\s*\)\s*$') {
        $sets[$currentName] = @($currentItems)
        $currentName = $null
        $currentItems = @()
        continue
    }

    $clean = ($line -replace "#.*$", "").Trim()
    if ($clean.Length -ne 0) {
        $currentItems += $clean.Trim('"')
    }
}

function Get-CMakeSetItems {
    param([string]$Name)

    if ($sets.ContainsKey($Name)) {
        return @($sets[$Name])
    }
    return @()
}

function Expand-CMakeItems {
    param(
        [string[]]$Items,
        [string[]]$Stack = @()
    )

    $expanded = @()
    foreach ($item in $Items) {
        if ($item -match '^\$\{([A-Za-z0-9_]+)\}$') {
            $varName = $matches[1]
            if ($Stack -contains $varName) {
                throw "Recursive CMake variable reference while expanding $varName"
            }
            $expanded += Expand-CMakeItems -Items (Get-CMakeSetItems -Name $varName) -Stack ($Stack + $varName)
        } else {
            $expanded += $item
        }
    }
    return $expanded
}

$variants = Get-CMakeSetItems -Name "SONICCD_ALL_VARIANTS"
if ($variants.Count -eq 0) {
    throw "No backend variants found in SONICCD_ALL_VARIANTS"
}

$variantRows = @()
$sourceMap = @{}

foreach ($variant in $variants) {
    $sources = Expand-CMakeItems -Items (Get-CMakeSetItems -Name "${variant}_SOURCES")
    $defines = Get-CMakeSetItems -Name "${variant}_DEFINES"
    $backendSources = @()

    foreach ($source in $sources) {
        $normalized = $source -replace "\\", "/"
        if ($normalized -like "src/frontend/*" -or $normalized -like "src/winexe/*") {
            continue
        }

        $backendSources += $normalized
        if (-not $sourceMap.ContainsKey($normalized)) {
            $sourceMap[$normalized] = [ordered]@{
                Source = $normalized
                Variants = @()
            }
        }
        $sourceMap[$normalized].Variants += $variant
    }

    $variantRows += [ordered]@{
        Variant = $variant
        SourceCount = $backendSources.Count
        Sources = $backendSources
        Defines = $defines
    }
}

$sourceRows = foreach ($key in ($sourceMap.Keys | Sort-Object)) {
    [ordered]@{
        Source = $key
        VariantCount = $sourceMap[$key].Variants.Count
        Variants = @($sourceMap[$key].Variants | Sort-Object -Unique)
    }
}

$inventory = [ordered]@{
    RepoRoot = (Resolve-Path -LiteralPath $RepoRoot).Path
    VariantCount = $variantRows.Count
    SourceCount = $sourceRows.Count
    Variants = $variantRows
    Sources = @($sourceRows)
}

if ($Format -eq "Json") {
    $output = $inventory | ConvertTo-Json -Depth 8
} else {
    $lines = New-Object System.Collections.Generic.List[string]
    $lines.Add("# Backend Source Inventory")
    $lines.Add("")
    $lines.Add("Variants: $($inventory.VariantCount)")
    $lines.Add("Unique backend sources: $($inventory.SourceCount)")
    $lines.Add("")
    $lines.Add("## Variants")
    $lines.Add("")
    foreach ($row in $variantRows) {
        $defineText = if ($row.Defines.Count) { $row.Defines -join ", " } else { "(none)" }
        $lines.Add("- ``$($row.Variant)``: $($row.SourceCount) source files; defines: $defineText")
    }
    $lines.Add("")
    $lines.Add("## Sources")
    $lines.Add("")
    foreach ($row in $sourceRows) {
        $lines.Add("- ``$($row.Source)``: $($row.Variants -join ", ")")
    }
    $output = $lines -join [Environment]::NewLine
}

if ($OutputPath.Length -ne 0) {
    $resolvedOutput = if ([System.IO.Path]::IsPathRooted($OutputPath)) {
        $OutputPath
    } else {
        Join-Path $RepoRoot $OutputPath
    }
    $parent = Split-Path -Parent $resolvedOutput
    if ($parent -and -not (Test-Path -LiteralPath $parent)) {
        New-Item -ItemType Directory -Path $parent | Out-Null
    }
    Set-Content -LiteralPath $resolvedOutput -Value $output
} else {
    Write-Output $output
}
