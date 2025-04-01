# Get the current directory
$currentDir = $PSScriptRoot

# Define source and destination paths
$sourcePath = Join-Path $currentDir "vs-build\plugin\AudioPlugin_artefacts\Debug\VST3\LightTone MS-18x.vst3"
$destPath = "C:\Program Files\Common Files\VST3"

# Check if source exists
if (-not (Test-Path $sourcePath)) {
    Write-Host "Error: VST3 plugin not found at $sourcePath"
    Write-Host "Please build the project first using 'cmake --build vs-build'"
    exit 1
}

# Check if destination exists, create if it doesn't
if (-not (Test-Path $destPath)) {
    Write-Host "Creating VST3 directory..."
    New-Item -ItemType Directory -Path $destPath -Force
}

# Copy the VST3 plugin
Write-Host "Installing VST3 plugin..."
Copy-Item -Path $sourcePath -Destination $destPath -Recurse -Force

Write-Host "Installation complete! The plugin has been installed to:"
Write-Host (Join-Path $destPath "LightTone MS-18x.vst3") 