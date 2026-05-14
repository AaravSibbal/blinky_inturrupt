1. change the printf from standard to https://github.com/mpaland/printf/blob/master/printf.c this is made for embedded and does not use the heap, there are some things that I need to figure out about this, like the buffer and things like that but that is not for now. (done);
2. make objects opaque
- gpio(done)
- itm (done)

3. deal with side effects for it. 
- gpio (done)

5. delay do that in general (done)

6. set up button device (curent)
    - connect the nvic, exti, syscfg, gpio, rcc dots. 

7. convert all enums that are used in bit operations to uint32_t.(done)

8. create masks for gpio bitshifts (done)

9. create critical sections for __IO variables when doing read/modify/write (done)

10. use Bit-banding for single bit write operations (the Difinitive guide Chapter 5.5) (done)

11. finish RCC driver with peripheral clocks and all (done)

12. set up LED device(done)
 
13. run main.(done) 
