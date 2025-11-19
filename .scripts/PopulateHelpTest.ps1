$output_path = Join-Path $PSScriptRoot -ChildPath ".." "Output"

$allfiles = Get-ChildItem -Path $output_path -File -Filter "*.exe" -Recurse
Write-Host $allfiles.Length
Write-Host $allfiles


$files = $allfiles | Where-Object { $_.FullName.contains("Release") }

Write-Host $files.Length
Write-Host $files



$files = $allfiles | Where-Object { $_.FullName.contains("Debug") }
Write-Host $files.Length
Write-Host $files
