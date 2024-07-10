# C_user_space_threads

C implementation of user space threads on x86-64 machine. \
Under the hood there's custom_ucontext.h lib that (kind of) implements ucontext.h API. 

For now it doesn't:
* handle signals
* support argument passing in makecontext 