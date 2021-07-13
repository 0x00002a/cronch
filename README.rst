cronch
======
*A tasty serialization library, C++20 required*


Introduction
------------

The aim of cronch is to provide a less painful way of serializing and deserializing C++ types.
It does not use reflection, so you still have to provide names for fields and such but the idea is 
you should only have to provide this information *once*. 2n times for each format you need. 

cronch is designed to be an interface layer of sorts and delegate the actual parsing and formatting of bytes 
to specialised libraries. To this end, its main goal is simply to take in the information about your types in 
as pain-free way as possible and expose it in a way that is easy to consume.



Disclaimer 
----------

cronch is currently in early development and not particularly stable. It should be usable but beware there may be API changes and 
occasional bugs.

Structure
---------

Rather than use traditional inheritance based dependency injection and interface definition, cronch relies on templates 
and concepts.

cronch is header-only and *very* template heavy. You have been warned. Having said that, it also makes use of C++20 
features such as concepts, which reduce the horrible error messages and the burden on the compiler from SFINAE.


Dependencies
-------------

- boost >=1.76.0

For each backend
#################

- JSON: nlomann/json 
- XML: pugixml




