$flags = "-Wall", "-Wextra", "-g", "-std=c11"
Write-Host "Building development version..."
gcc $flags -o interpreter.exe interpreter.c

if ($LASTEXITCODE -eq 0) {
    Write-Host "Build successful! Running..."
    ./interpreter.exe
} else {
    Write-Host "Compilation failed with errors!" -ForegroundColor Red
}