# kenwood_xs8_sl16
 Arduino test sketch for Kenwood system remote control XS8/SL16

```
for Kenwood KXF-W4010

CMD	        XS8	            SL16
-----------------------------------------------------
ff B 	 0100 0010 (0x42) 11111010 0010 0100 (0xFA24)
ff A 	 0100 0011 (0x43) 11111010 0011 0100 (0xFA34)
stop B 	 0100 0100 (0x44) 11111010 0100 0100 (0xFA44)
stop A 	 0100 0101 (0x45) 11111010 0101 0100 (0xFA54)
> B 	 0100 0110 (0x46) 11111010 0110 0100 (0xFA64)
> A 	 0100 0111 (0x47) 11111010 0111 0100 (0xFA74)
rec B 	 0100 1000 (0x48) 11111010 1000 0100 (0xFA84)
rew B 	 0100 1010 (0x4A) 11111010 1010 0100 (0xFAA4)
rew A 	 0100 1011 (0x4B) 11111010 1011 0100 (0xFAB4)
pause B  0100 1100 (0x4C) 11111010 1100 0100 (0xFAC4)
< B 	 0100 1110 (0x4E) 11111010 1110 0100 (0xFAE4)
< A 	 0100 1111 (0x4F) 11111010 1111 0100 (0xFAF4)
```