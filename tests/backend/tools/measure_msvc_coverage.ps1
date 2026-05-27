param(
    [string]$RepoRoot = (Resolve-Path "$PSScriptRoot\..\..\..").Path,
    [string]$BuildDir = "build-backend-tests-win32",
    [string]$Config = "Debug",
    [string]$Variants = "R11A",
    [string]$CoverageTool = "",
    [int]$Jobs = 0,
    [switch]$SkipConfigure,
    [switch]$SkipBuild
)

$ErrorActionPreference = "Stop"

function Find-CodeCoverageConsole {
    param([string]$ExplicitPath)

    if ($ExplicitPath.Length -ne 0) {
        if (-not (Test-Path -LiteralPath $ExplicitPath)) {
            throw "Coverage tool not found: $ExplicitPath"
        }
        return (Resolve-Path -LiteralPath $ExplicitPath).Path
    }

    $candidates = @(
        "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\Extensions\Microsoft\CodeCoverage.Console\Microsoft.CodeCoverage.Console.exe",
        "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\Extensions\Microsoft\CodeCoverage.Console\Microsoft.CodeCoverage.Console.exe"
    )

    foreach ($candidate in $candidates) {
        if (Test-Path -LiteralPath $candidate) {
            return $candidate
        }
    }

    $roots = @(
        "C:\Program Files\Microsoft Visual Studio",
        "${env:ProgramFiles(x86)}\Microsoft Visual Studio"
    )

    foreach ($root in $roots) {
        if (-not (Test-Path -LiteralPath $root)) {
            continue
        }
        $found = Get-ChildItem -Path $root -Recurse -Filter Microsoft.CodeCoverage.Console.exe -ErrorAction SilentlyContinue |
            Select-Object -First 1
        if ($found) {
            return $found.FullName
        }
    }

    throw "Microsoft.CodeCoverage.Console.exe was not found. Install Visual Studio code coverage tools or pass -CoverageTool."
}

function Get-BackendCoverageSummary {
    param(
        [string]$XmlPath,
        [string]$Root
    )

    [xml]$coverage = Get-Content -LiteralPath $XmlPath
    $sourceStats = @{}
    $rootPath = (Resolve-Path -LiteralPath $Root).Path
    $srcPrefix = (Join-Path $rootPath "src").ToLowerInvariant()
    $frontendPrefix = (Join-Path $rootPath "src\frontend").ToLowerInvariant()
    $winexePrefix = (Join-Path $rootPath "src\winexe").ToLowerInvariant()

    foreach ($module in $coverage.results.modules.module) {
        $sourceById = @{}
        foreach ($source in $module.source_files.source_file) {
            $path = $source.path
            if (-not $path) {
                continue
            }
            $sourceById[$source.id] = $path
        }

        foreach ($function in $module.functions.function) {
            foreach ($range in $function.ranges.range) {
                if (-not $sourceById.ContainsKey($range.source_id)) {
                    continue
                }

                $path = $sourceById[$range.source_id]
                $lowerPath = $path.ToLowerInvariant()
                if (-not $lowerPath.StartsWith($srcPrefix)) {
                    continue
                }
                if ($lowerPath.StartsWith($frontendPrefix) -or $lowerPath.StartsWith($winexePrefix)) {
                    continue
                }

                $rootUri = New-Object System.Uri(($rootPath.TrimEnd('\') + '\'))
                $pathUri = New-Object System.Uri($path)
                $relative = [System.Uri]::UnescapeDataString($rootUri.MakeRelativeUri($pathUri).ToString())
                if (-not $sourceStats.ContainsKey($relative)) {
                    $sourceStats[$relative] = [ordered]@{
                        File = $relative
                        Covered = New-Object 'System.Collections.Generic.HashSet[int]'
                        Partial = New-Object 'System.Collections.Generic.HashSet[int]'
                        Uncovered = New-Object 'System.Collections.Generic.HashSet[int]'
                    }
                }

                $start = [int]$range.start_line
                $end = [int]$range.end_line
                for ($line = $start; $line -le $end; ++$line) {
                    switch ($range.covered) {
                        "yes" {
                            [void]$sourceStats[$relative].Covered.Add($line)
                            [void]$sourceStats[$relative].Partial.Remove($line)
                            [void]$sourceStats[$relative].Uncovered.Remove($line)
                        }
                        "partial" {
                            if (-not $sourceStats[$relative].Covered.Contains($line)) {
                                [void]$sourceStats[$relative].Partial.Add($line)
                                [void]$sourceStats[$relative].Uncovered.Remove($line)
                            }
                        }
                        default {
                            if (-not $sourceStats[$relative].Covered.Contains($line) -and
                                -not $sourceStats[$relative].Partial.Contains($line)) {
                                [void]$sourceStats[$relative].Uncovered.Add($line)
                            }
                        }
                    }
                }
            }
        }
    }

    $rows = foreach ($entry in ($sourceStats.GetEnumerator() | Sort-Object Name)) {
        $covered = $entry.Value.Covered.Count
        $partial = $entry.Value.Partial.Count
        $uncovered = $entry.Value.Uncovered.Count
        $total = $covered + $partial + $uncovered
        $coveredForPercent = $covered + $partial
        $percent = if ($total -eq 0) { 100.0 } else { 100.0 * $coveredForPercent / $total }
        [pscustomobject]@{
            File = $entry.Value.File
            Covered = $covered
            Partial = $partial
            Uncovered = $uncovered
            Total = $total
            LineCoverage = [math]::Round($percent, 2)
        }
    }

    return @($rows)
}

$repoPath = (Resolve-Path -LiteralPath $RepoRoot).Path
$buildPath = if ([System.IO.Path]::IsPathRooted($BuildDir)) {
    $BuildDir
} else {
    Join-Path $repoPath $BuildDir
}
$coveragePath = Join-Path $buildPath "backend-coverage.xml"
$coverageToolPath = Find-CodeCoverageConsole -ExplicitPath $CoverageTool
$parallelJobs = if ($Jobs -eq 0) {
    [Math]::Min([Environment]::ProcessorCount, 8)
} else {
    $Jobs
}
if ($parallelJobs -lt 1) {
    throw "-Jobs must be 0 for auto or a positive integer."
}
Write-Output "Using $parallelJobs parallel job(s)."

if (-not $SkipConfigure) {
    & cmake -S $repoPath -B $buildPath -A Win32 -DSONICCD_BUILD_TESTS=ON "-DSONICCD_VARIANTS=$Variants"
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

if (-not $SkipBuild) {
    & cmake --build $buildPath --config $Config --target backend-tests --parallel $parallelJobs
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

& $coverageToolPath collect --nologo --output $coveragePath --output-format xml ctest --test-dir $buildPath -C $Config --output-on-failure --parallel $parallelJobs
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

$rows = Get-BackendCoverageSummary -XmlPath $coveragePath -Root $repoPath
$covered = ($rows | Measure-Object -Property Covered -Sum).Sum
$partial = ($rows | Measure-Object -Property Partial -Sum).Sum
$uncovered = ($rows | Measure-Object -Property Uncovered -Sum).Sum
$total = $covered + $partial + $uncovered
$percent = if ($total -eq 0) { 100.0 } else { 100.0 * ($covered + $partial) / $total }

Write-Output ""
Write-Output "Backend source coverage"
Write-Output "Coverage XML: $coveragePath"
Write-Output ("Line coverage: {0:N2}% ({1}/{2} covered-or-partial lines, {3} uncovered)" -f $percent, ($covered + $partial), $total, $uncovered)
Write-Output ""
$rows | Sort-Object LineCoverage, File | Format-Table File, LineCoverage, Covered, Partial, Uncovered, Total -AutoSize
