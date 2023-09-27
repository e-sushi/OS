global test:function (test.end - test)
test:
    movups xmm0, [rsi]
    movups [rsi], xmm0
.end:


