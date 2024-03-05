This little script helps you monitor how busy your CPU is. It works by taking snapshots of CPU activity at regular intervals and then calculating the percentage of time it was actually working.

## How to compile

`gcc getcpu.c -o getcpu -lm`

That's it.

## How to use

./getcpu <time_in_milliseconds> [r]

  <time_in_milliseconds>: This is how often (in milliseconds) you want the script to check CPU usage. The minimum is 200ms, but 900ms (0.9 seconds) is recommended.
  
  [r]: This is optional. If you add it, the reported CPU usage will be rounded to the nearest whole number instead of showing decimals.

## Examples:

This will print the CPU usage every second (1000 milliseconds) with two decimal places.

`$ ./getcpu 1000`
 
\> 98.32


 
This will print the CPU usage every second, (900 milliseconds) and the percentage will be rounded.

 `$ ./getcpu 900 r`
 
 \> 98 



## How it works

  The script reads information from a special file called /proc/stat that contains details about your CPU's activity.
    It takes two readings of this file at the specified interval and calculates the difference.
    Based on this difference, it figures out what percentage of the time the CPU was actually working.
    Finally, it prints the CPU usage percentage to the terminal.
