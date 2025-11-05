$content = Get-Content 'D:\RiderProjects\Socket\Client\Content\UI\ItemIconSet.png.meta' -Raw
$content = $content -replace 'width:\s*\d+', 'width: 32'
$content = $content -replace 'height:\s*\d+', 'height: 32'
Set-Content 'D:\RiderProjects\Socket\Client\Content\UI\ItemIconSet.png.meta' -Value $content -NoNewline
Write-Host "width와 height 값을 모두 32로 변경했습니다."
