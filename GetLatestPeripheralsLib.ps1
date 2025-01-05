$repo = "fabcolonna/polito-cas-landtiger-lib"
$dir = ".\Keil\Libs\Peripherals\"
$pattern = "peripherals-lib.tar.gz"

# Checking if GH CLI is installed
Get-Command gh -ErrorAction SilentlyContinue | Out-Null
if (-not $?) 
{
    Write-Host "Error: GitHub CLI (gh) is required, but not installed."
    exit
}

# Checking if tar is installed
Get-Command tar -ErrorAction SilentlyContinue | Out-Null
if (-not $?) 
{
    Write-Host "Error: tar is required, but not installed."
    exit
}

Write-Host "Authenticating with GitHub CLI..."
gh auth status > $null || gh auth login

gh repo view $repo | Out-Null
if (-not $?) 
{
    Write-Host "Error: You don't have access to the repository $repo."
    exit
}

New-Item -ItemType File -Path $dir$pattern -Force | Out-Null
Remove-Item $dir* -Recurse -Force | Out-Null
gh release download --repo $repo --pattern $pattern --dir $dir
tar --strip-components=1 --exclude='._*' -xf $dir$pattern -C $dir
Remove-Item $dir$pattern
Write-Host "Done."

