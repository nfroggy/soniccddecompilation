param(
    [string]$RepoRoot = (Resolve-Path "$PSScriptRoot\..\..\..").Path,
    [ValidateSet("Markdown", "Json", "Csv")]
    [string]$Format = "Markdown",
    [ValidateSet("VariantOnly", "MSVC")]
    [string]$DefineMode = "VariantOnly",
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
            $rest = ($matches[2] -replace "#.*$", "").Trim()
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

function Get-DefineKey {
    param([string[]]$Defines)

    if ($Defines.Count -eq 0) {
        return ""
    }
    return (@($Defines | Sort-Object) -join "`n")
}

$variants = Get-CMakeSetItems -Name "SONICCD_ALL_VARIANTS"
if ($variants.Count -eq 0) {
    throw "No backend variants found in SONICCD_ALL_VARIANTS"
}

$commonDefines = @()
if ($DefineMode -eq "MSVC") {
    $commonDefines = @("_CRT_SECURE_NO_WARNINGS", "WIN32", "_WINDOWS")
}

$sourceCombos = @{}
$variantRows = @()

foreach ($variant in $variants) {
    $sources = Expand-CMakeItems -Items (Get-CMakeSetItems -Name "${variant}_SOURCES")
    if ($sources.Count -eq 0) {
        throw "No source list found for variant $variant"
    }

    $defines = @($commonDefines + (Get-CMakeSetItems -Name "${variant}_DEFINES") | Sort-Object -Unique)
    $defineKey = Get-DefineKey -Defines $defines
    $backendSources = @()

    foreach ($source in $sources) {
        $normalized = $source -replace "\\", "/"
        if ($normalized -like "src/frontend/*" -or $normalized -like "src/winexe/*") {
            continue
        }

        $backendSources += $normalized
        if (-not $sourceCombos.ContainsKey($normalized)) {
            $sourceCombos[$normalized] = @{}
        }
        if (-not $sourceCombos[$normalized].ContainsKey($defineKey)) {
            $sourceCombos[$normalized][$defineKey] = [ordered]@{
                Defines = @($defines)
                Variants = @()
            }
        }
        $sourceCombos[$normalized][$defineKey].Variants += $variant
    }

    $variantRows += [ordered]@{
        Variant = $variant
        SourceCount = $backendSources.Count
        Defines = @($defines)
        Sources = @($backendSources | Sort-Object -Unique)
    }
}

$sourceRows = foreach ($source in ($sourceCombos.Keys | Sort-Object)) {
    $combos = foreach ($comboKey in ($sourceCombos[$source].Keys | Sort-Object)) {
        [ordered]@{
            Defines = @($sourceCombos[$source][$comboKey].Defines)
            VariantCount = ($sourceCombos[$source][$comboKey].Variants | Sort-Object -Unique).Count
            Variants = @($sourceCombos[$source][$comboKey].Variants | Sort-Object -Unique)
        }
    }

    [ordered]@{
        Source = $source
        CombinationCount = @($combos).Count
        DefineCombinations = @($combos)
    }
}

$matrix = [ordered]@{
    RepoRoot = (Resolve-Path -LiteralPath $RepoRoot).Path
    DefineMode = $DefineMode
    VariantCount = $variantRows.Count
    SourceCount = @($sourceRows).Count
    Variants = @($variantRows)
    Sources = @($sourceRows)
}

if ($Format -eq "Json") {
    $output = $matrix | ConvertTo-Json -Depth 10
} elseif ($Format -eq "Csv") {
    $flatRows = foreach ($source in $sourceRows) {
        foreach ($combo in $source.DefineCombinations) {
            [pscustomobject]@{
                Source = $source.Source
                Defines = if ($combo.Defines.Count) { $combo.Defines -join ";" } else { "" }
                Variants = $combo.Variants -join ";"
                VariantCount = $combo.VariantCount
            }
        }
    }
    $output = $flatRows | ConvertTo-Csv -NoTypeInformation
} else {
    $lines = New-Object System.Collections.Generic.List[string]
    $lines.Add("# Backend Source Define Matrix")
    $lines.Add("")
    $lines.Add("Define mode: ``$DefineMode``")
    $lines.Add("Variants: $($matrix.VariantCount)")
    $lines.Add("Unique backend sources: $($matrix.SourceCount)")
    $lines.Add("")
    $lines.Add("| Source | Defines | Variants |")
    $lines.Add("| --- | --- | --- |")
    foreach ($source in $sourceRows) {
        foreach ($combo in $source.DefineCombinations) {
            $defineText = if ($combo.Defines.Count) { "``" + ($combo.Defines -join "``<br>``") + "``" } else { "(none)" }
            $variantText = "``" + ($combo.Variants -join "``<br>``") + "``"
            $lines.Add("| ``$($source.Source)`` | $defineText | $variantText |")
        }
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
