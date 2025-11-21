# Script tao anh minh hoa don gian cho cac dich vu
$imageDir = "d:\PBL2-\Data\images"

if (-not (Test-Path $imageDir)) {
    New-Item -ItemType Directory -Path $imageDir | Out-Null
}

# Ham tao SVG voi chu
function Create-ServiceIcon {
    param($filename, $text, $bgcolor)
    
    $svg = @"
<svg xmlns='http://www.w3.org/2000/svg' width='60' height='60' viewBox='0 0 60 60'>
  <rect fill='$bgcolor' width='60' height='60' rx='8'/>
  <circle cx='30' cy='30' r='20' fill='rgba(255,255,255,0.2)'/>
  <text x='30' y='36' text-anchor='middle' fill='white' font-size='12' font-weight='bold' font-family='Arial'>$text</text>
</svg>
"@
    
    $filePath = Join-Path $imageDir $filename
    $svg | Out-File -Encoding UTF8 $filePath
    Write-Host "Created: $filename"
}

# Tao cac icon
Create-ServiceIcon "coca.svg" "COCA" "#e74c3c"
Create-ServiceIcon "pepsi.svg" "PEPSI" "#2e5090"
Create-ServiceIcon "sting.svg" "STING" "#ffd700"
Create-ServiceIcon "redbull.svg" "RED" "#1e3a8a"
Create-ServiceIcon "nuoc_suoi.svg" "AQUA" "#3b82f6"
Create-ServiceIcon "tra_dao.svg" "TRA" "#fb923c"
Create-ServiceIcon "tra_sua.svg" "MILK" "#92400e"
Create-ServiceIcon "cafe_den.svg" "CAFE" "#422006"
Create-ServiceIcon "cafe_sua.svg" "CAFE" "#a16207"
Create-ServiceIcon "nuoc_cam.svg" "CAM" "#f97316"
Create-ServiceIcon "nuoc_ep.svg" "JUICE" "#10b981"
Create-ServiceIcon "soda.svg" "SODA" "#06b6d4"
Create-ServiceIcon "bong_da.svg" "BALL" "#000000"
Create-ServiceIcon "gang_tay.svg" "GLOVE" "#059669"
Create-ServiceIcon "ao_dau.svg" "SHIRT" "#dc2626"
Create-ServiceIcon "quan_dau.svg" "PANTS" "#2563eb"
Create-ServiceIcon "giay_da_bong.svg" "SHOES" "#374151"
Create-ServiceIcon "tat_bong.svg" "SOCK" "#6b7280"
Create-ServiceIcon "luoi_khung.svg" "NET" "#9ca3af"
Create-ServiceIcon "cot_phu.svg" "FLAG" "#f59e0b"
Create-ServiceIcon "con.svg" "CON" "#a855f7"
Create-ServiceIcon "bang_dieu_khien.svg" "BOARD" "#14532d"
Create-ServiceIcon "the_vang.svg" "CARD" "#fbbf24"
Create-ServiceIcon "the_do.svg" "CARD" "#dc2626"
Create-ServiceIcon "coi.svg" "WHIST" "#111827"
Create-ServiceIcon "binh_nuoc.svg" "BOTTLE" "#60a5fa"

Write-Host "`nCompleted! Created 26 image files in $imageDir"
