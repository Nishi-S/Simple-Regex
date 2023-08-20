$test = Get-ChildItem ".\test\\test*.ps1" -Exclude $MyInvocation.MyCommand

$test | ForEach-Object { &$_ }