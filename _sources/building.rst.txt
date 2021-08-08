Building
==========

The library itself is header-only, but some parts of it do require linking to compiled dependencies. Currently these parts are:

- Boost.JSON backend support (`cronch::json::boost`) 

CMake options
###############

.. list-table:: 
   :widths: 25 20 7
   :header-rows: 1 

   * 
     - Description 
     - Name 
     - Default
   
   * 
     - Builds the tests 
     - CRONCH_BUILD_TESTS 
     - OFF 
   * 
     - Builds the examples 
     - CRONCH_BUILD_EXAMPLES 
     - OFF 
   * 
     - Pull in dependencies automatically via conan 
     - CRONCH_USE_CONAN 
     - OFF
   *
     - Link to Boost.JSON transistively and include `cronch::json::boost` support 
     - CRONCH_FEATURE_BOOST_JSON 
     - ON


