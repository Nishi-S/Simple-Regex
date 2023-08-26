
$compiler = Get-ChildItem ".\x64\Release\*compile*.exe" 
$vm = Get-ChildItem ".\x64\Release\*vm*.exe" 

function expect
{
    param (
        $regex,
        $text,
        $expect,
        [switch]$notMatch
    )
    
    $ret = &$compiler $regex | &$vm $text
    if ($ret -ne $expect)
    {
        Write-Host "`"$regex`" => `"$text`" " -NoNewline 
        if ($notMatch)
        {
            Write-Host "expect not matched, but got $ret"
        }
        else
        {
            Write-Host "expect $expect, but got $ret"
        }
    }
    else
    {
        Write-Host "OK" 
    }
}

Write-Host "============ character class test ============"
expect "[abc]" "a" "a"
expect "[abc]" "b" "b"
expect -notMatch "[abc]" "x" 
expect "[0-9]" "5" "5"
expect -notMatch "[0-9]" "a" 
expect "[A-Za-z0-9]" "3" "3"
expect -notMatch "[A-Za-z0-9]" "%"

expect "[0-9]+" "123456789054" "123456789054"
expect -notMatch "[0-9]+" "abc"
expect "[A-Za-z]*" "HelloWorld" "HelloWorld"
expect -notMatch "[A-Za-z]+" "123"
expect "[A-Za-z]*" "123" ""
expect "[0-9]+[a-f]+" "123abc" "123abc"
expect -notMatch "[0-9]+[a-f]+" "123XYZX"