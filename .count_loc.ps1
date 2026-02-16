# Count lines of code (C/C++/headers/Python/Lua) excluding files listed in .gitattributes
$root = (Get-Location).Path

# Read exclude entries (first token on each line) and normalize to forward-slash relative paths
$excludes = @()
if (Test-Path ".gitattributes") {
  Get-Content .gitattributes | ForEach-Object {
    $tok = ($_ -split '\s+')[0]
    if ($tok -and $tok.Trim() -ne '') {
      $excludes += ($tok -replace '\\','/')
    }
  }
}

# File extensions to count
$exts = '*.c','*.cpp','*.cc','*.cxx','*.h','*.hpp','*.hh','*.py','*.lua'

# Gather candidate files
$files = Get-ChildItem -Path $root -Recurse -File -Include $exts | Sort-Object FullName

# Filter out excluded files by comparing relative path with forward-slashes
$files = $files | Where-Object {
  $rel = $_.FullName.Substring($root.Length+1) -replace '\\','/'
  -not ($excludes -contains $rel)
}

$total = 0
$byExt = @{}
$fileCounts = @()

foreach ($f in $files) {
  try {
    $count = (Get-Content $f.FullName -ErrorAction Stop | Measure-Object -Line).Lines
  } catch {
    $count = 0
  }

  $total += $count
  $ext = $f.Extension.ToLower()
  if (-not $byExt.ContainsKey($ext)) { $byExt[$ext] = 0 }
  $byExt[$ext] += $count

  $rel = $f.FullName.Substring($root.Length+1) -replace '\\','/'
  $fileCounts += [PSCustomObject]@{Lines=$count; Rel=$rel; File=$f.FullName}
}

Write-Output "TotalLines=$total"
Write-Output "FilesCount=$($files.Count)"
foreach ($k in $byExt.Keys | Sort-Object) { Write-Output "$k $($byExt[$k])" }

# Top 20 files by lines
Write-Output ""
Write-Output "--Top 20 files by line count--"
$fileCounts | Sort-Object Lines -Descending | Select-Object -First 20 | Format-Table @{Label='Lines';Expression={$_.Lines}}, @{Label='Path';Expression={$_.Rel}} -AutoSize

# Also print excluded list for verification
Write-Output ""
Write-Output "--Excluded (from .gitattributes)--"
$excludes | ForEach-Object { Write-Output $_ }
