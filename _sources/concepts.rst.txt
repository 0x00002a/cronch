Concepts 
========

*As in the C++20 feature*


.. namespace:: cronch

.. namespace-push:: concepts

.. _General concepts:

General concepts
------------------

.. concept:: template<typename T> serializable

   Type that can be serialized by cronch at the most basic level.

   **Requirements**
        - ``{ metadata<T>::name } -> std::same_as<const char*>``

.. concept:: template<typename T> has_members

   Type that can defines fields for use by cronch

   **Requirements** 
      - :concept:`serializable`
      - ``{ metadata<T>::members } -> meta::concepts::view``


.. concept:: template<typename T> known_to_cronch 

   Type that satisfies all other concepts for meta information


   **Requirements** 
    - :concept:`serializable`
    - :concept:`has_members`

.. concept:: template<typename B> backend 

   Type that can be used as a serializing/deserializing backend.  

   **Requirements** 
    - B::append(typename B::document_type&, const known_to_cronch auto&)
    - b.parse_into(known_to_cronch auto&)

   **Implementations** 
    - :class:`pugi`
    - :class:`nloh`

.. concept:: template<typename C> iterable 

   Type that can be iterated over 

   **Requirements**
    - :expr:`std::begin(c)`
    - :expr:`std::end(c)`

.. namespace-pop::


.. namespace-push:: meta::concepts

.. _metadata concepts:

Metadata concepts
-----------------

.. concept:: template<typename G> getter 

   A type that acts as a getter to a member of another type
    
   **Requirements** 
    - :expr:`typename G::owning_type`
    - :expr:`typename G::value_type`
    - :expr:`std::invocable<G, const typename G::owning_type&>`
    - :expr:`std::same_as<std::invoke_result_t<G, const typename G::owning_type&>, typename G::value_type>`

   **Implementations** 
    - Anything that implements :concept:`accessor`

.. concept:: template<typename S> setter 

   A type that acts as a setter to a member of another type 

   **Requirements**
    - :expr:`typename S::owning_type`
    - :expr:`typename S::value_type`
    - :expr:`std::invocable<S, typename G::owning_type&, typename S::value_type>`

   **Implementations**
    - Anything that implements :concept:`accessor`

.. concept:: template<typename A> accessor 

   A type that acts as both a setter and getter for a member of another type 

   **Requirements** 
    - :concept:`setter`
    - :concept:`getter`

   **Implementations** 
    - :class:`field`
    - :any:`property`

.. concept:: template<typename V> view 

   A type that provides a view over the attributes of another type 

   **Requirements**
    - v.map([]<typename T>(T&&) requires(getter<T> || setter<T>){})

   **Implementations**
    - :class:`mems`

    
