#!/usr/bin/expect    

# used to syncronize and kill programs

set passwd "redhat"
set timeout 3

set user [lindex $argv [expr $argc-2]]
set mode [lindex $argv [expr $argc-1]]
spawn ssh -l root $user
expect "y/n" {send "y\r"}
expect "password:" {send "$passwd\n"}
#expect "*#"
puts "$mode"
#expect "*#"
if { $mode == "sync" } {
	send "sync\n"
	expect "*#" 
	send "reboot\n"
} elseif { $mode == "car" } {
	expect "*#"	{send -- {ps | grep [C]arController | awk '{print $1}' | xargs kill -9};send -- \n}
} elseif { $mode == "sub" } {
	expect "*#"	{send -- {ps | grep [V]ideoSubServer | awk '{print $1}' | xargs kill -9 1>/dev/null 2>&1};send -- \n}
} elseif { $mode == "room" } {
	expect "*#"	{send -- {ps | grep [R]oomMedia | awk '{print $1}' | xargs kill -9 1>/dev/null 2>&1};send -- \n}
	expect "*#"	{send -- {ps | grep [r]ecipeclient | awk '{print $1}' | xargs kill -9 1>/dev/null 2>&1};send -- \n}
}
expect "*#" {send "exit\n"}
interact

