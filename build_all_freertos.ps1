$workspace = "D:\ProgramFiles\ESPIDF\WorkSpace"

$results = @()

$projects = Get-ChildItem $workspace -Directory |
    Where-Object { $_.Name -like "frtos_*" } |
    Sort-Object Name

Write-Host "===================================================" -ForegroundColor Cyan
Write-Host "        FreeRTOS 90 Project Build" -ForegroundColor Cyan
Write-Host "===================================================" -ForegroundColor Cyan
Write-Host "Projects found : $($projects.Count)"
Write-Host "Workspace       : $workspace"
Write-Host ""

foreach ($project in $projects)
{
    Write-Host "---------------------------------------------------" -ForegroundColor Yellow
    Write-Host "Building: $($project.Name)" -ForegroundColor Yellow
    Write-Host "---------------------------------------------------"

    Set-Location $project.FullName

    $startTime = Get-Date

    & idf.py build

    $exitCode = $LASTEXITCODE

    $endTime = Get-Date

    $duration = $endTime - $startTime

    if ($exitCode -eq 0)
    {
        Write-Host "BUILD SUCCESS: $($project.Name)" -ForegroundColor Green

        $status = "SUCCESS"
    }
    else
    {
        Write-Host "BUILD FAILED: $($project.Name)" -ForegroundColor Red

        $status = "FAILED"
    }

    $results += [PSCustomObject]@{
        Project  = $project.Name
        Status   = $status
        Duration = $duration.ToString("hh\:mm\:ss")
    }

    Write-Host ""
}

Set-Location $workspace

Write-Host ""
Write-Host "===================================================" -ForegroundColor Cyan
Write-Host "                 BUILD SUMMARY" -ForegroundColor Cyan
Write-Host "===================================================" -ForegroundColor Cyan

$results | Format-Table -AutoSize

$successCount = ($results | Where-Object Status -eq "SUCCESS").Count
$failedCount  = ($results | Where-Object Status -eq "FAILED").Count

Write-Host ""
Write-Host "Total   : $($results.Count)"
Write-Host "Success : $successCount" -ForegroundColor Green
Write-Host "Failed  : $failedCount" -ForegroundColor Red
Write-Host "==================================================="



