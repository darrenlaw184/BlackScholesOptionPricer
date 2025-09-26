param([switch]$Force)

Write-Host "Setting up dependencies..." -ForegroundColor Cyan

if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
    Write-Host "Error: Git not found" -ForegroundColor Red
    exit 1
}

$external = "external"
if (-not (Test-Path $external)) {
    New-Item -ItemType Directory -Path $external | Out-Null
}

$imgui = "$external/imgui"
if ((Test-Path $imgui) -and (-not $Force)) {
    Write-Host "ImGui exists, skipping" -ForegroundColor Green
} else {
    if (Test-Path $imgui) { Remove-Item $imgui -Recurse -Force }
    git clone --depth 1 https://github.com/ocornut/imgui.git $imgui
}

$implot = "$external/implot"
if ((Test-Path $implot) -and (-not $Force)) {
    Write-Host "ImPlot exists, skipping" -ForegroundColor Green
} else {
    if (Test-Path $implot) { Remove-Item $implot -Recurse -Force }
    git clone --depth 1 https://github.com/epezent/implot.git $implot
}

Write-Host "Setup complete!" -ForegroundColor Green