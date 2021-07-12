CRONCH 
======
*The tastiest serialisation library... ish*


Introduction
------------

The aim of cronch is to provide a less painful way of serialisating and deserialsing C++ types.
It does not use reflection, so you still have to provide names for fields and such but the idea is 
you should only have to provide this information *once*. Rather than at least twice for serialisating and 
deserialsing.


Structure
---------

cronch is designed to be an interface layer of sorts and delegate the actual parsing and formatting of bytes 
to specialised libraries. To this end, its main goal is simply to take in the information about your types in 
as pain-free way as possible and expose it in a way that is easy to consume.


cronch is header-only and *very* template heavy. You have been warned.




