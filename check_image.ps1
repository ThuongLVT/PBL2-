Add-Type -AssemblyName System.Drawing
$imgPath = "D:\QT_PBL2\UI\Resources\images\peakpx.jpg"
$img = [System.Drawing.Image]::FromFile($imgPath)
$file = Get-Item $imgPath

Write-Host "=================================="
Write-Host "Image Information:"
Write-Host "=================================="
Write-Host "Width:  $($img.Width) px"
Write-Host "Height: $($img.Height) px"
Write-Host "Size:   $([math]::Round($file.Length/1MB, 2)) MB"
Write-Host "=================================="

$img.Dispose()
