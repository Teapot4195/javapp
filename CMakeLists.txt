cmake_minimum_required(VERSION 3.31)
project(javapp)

set(CMAKE_CXX_STANDARD 26)

option(JAVAPP_ENABLE_LEAK_TRACING "Enable leak tracing for the java++ library, including projects built using it." OFF)
option(JAVAPP_ENABLE_LEAK_STACKTRACE "Enable leak tracing with stacktraces, this is quite expensive, best not to turn it on when not neccesary" OFF)
option(JAVAPP_ENABLE_INTERNAL_ASSERTIONS "Enable the assertions of internal structures, may cause some performance issues." OFF)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    doxygen_add_docs(docs src ALL)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if(JAVAPP_ENABLE_LEAK_TRACING)
        add_compile_definitions(JAVAPP_TRACE_ALLOCS)
        if (JAVAPP_ENABLE_LEAK_STACKTRACE)
            add_compile_definitions(JAVAPP_TRACE_ALLOCS_STACKTRACE)
            set(CMAKE_ENABLE_EXPORTS TRUE)
            add_compile_options(-fno-omit-frame-pointer)
        endif()
    endif()
else()
    message("Leak Tracing is enabled, but is not supported on non linux systems (yet)!")
endif()

if (JAVAPP_ENABLE_INTERNAL_ASSERTIONS)
    add_compile_definitions(JAVAPP_ENABLE_INTERNAL_ASSERTIONS)
endif()

add_compile_options(-Wall)

add_library(javapp STATIC
        src/Object.cpp
        src/Object.h
        src/jlang.h
        src/Throwable.cpp
        src/Throwable.h
        src/String.cpp
        src/String.h
        src/AutoCloseable.h
        src/java/util/function/Consumer.cpp
        src/java/util/function/Consumer.h
        src/java/util/Iterator.cpp
        src/java/util/Iterator.h
        src/Runnable.cpp
        src/Runnable.h
        src/java/util/stream/BaseStream.h
        src/java/util/Comparator.cpp
        src/java/util/Comparator.h
        src/java/util/Spliterator.cpp
        src/java/util/Spliterator.h
        src/java/util/function/IntPredicate.cpp
        src/java/util/function/IntPredicate.h
        src/java/util/function/Supplier.cpp
        src/java/util/function/Supplier.h
        src/java/util/function/DoubleConsumer.cpp
        src/java/util/function/DoubleConsumer.h
        src/java/util/function/DoubleSupplier.cpp
        src/java/util/function/DoubleSupplier.h
        src/java/util/function/DoublePredicate.cpp
        src/java/util/function/DoublePredicate.h
        src/java/util/DoubleSummaryStatistics.cpp
        src/java/util/DoubleSummaryStatistics.h
        src/Double.cpp
        src/Double.h
        src/Iterable.cpp
        src/Iterable.h
        src/Long.cpp
        src/Long.h
        src/java/util/Collection.cpp
        src/java/util/Collection.h
        src/java/util/stream/Stream.cpp
        src/java/util/stream/Stream.h
        src/java/util/function/Predicate.cpp
        src/java/util/function/Predicate.h
        src/java/util/Objects.cpp
        src/java/util/Objects.h
        src/Array.h
        src/java/util/function/IntFunction.cpp
        src/java/util/function/IntFunction.h
        src/java/util/Spliterators.cpp
        src/java/util/Spliterators.h
        src/java/util/ListIterator.h
        src/java/util/function/Function.cpp
        src/java/util/function/Function.h
        src/java/util/function/UnaryOperator.cpp
        src/java/util/function/UnaryOperator.h
        src/java/util/List.cpp
        src/java/util/List.h
        src/java/util/RandomAccess.h
        src/java/util/function/BiConsumer.cpp
        src/java/util/function/BiConsumer.h
        src/java/util/Set.cpp
        src/java/util/Set.h
        src/java/util/function/BinaryOperator.cpp
        src/java/util/function/BinaryOperator.h
        src/java/util/function/BiFunction.cpp
        src/java/util/function/BiFunction.h
        src/Enum.cpp
        src/Enum.h
        src/java/io/Serializable.cpp
        src/java/io/Serializable.h
        src/Comparable.h
        src/java/util/stream/Collector.cpp
        src/java/util/stream/Collector.h
        src/java/util/OptionalDouble.cpp
        src/java/util/OptionalDouble.h
        src/java/util/stream/DoubleStream.cpp
        src/java/util/stream/DoubleStream.h
        src/java/util/Optional.cpp
        src/java/util/Optional.h
        src/java/util/stream/IntStream.cpp
        src/java/util/stream/IntStream.h
        src/java/util/stream/LongStream.cpp
        src/java/util/stream/LongStream.h
        src/java/util/function/IntConsumer.cpp
        src/java/util/function/IntConsumer.h
        src/java/util/function/LongConsumer.cpp
        src/java/util/function/LongConsumer.h
        src/java/util/function/ToDoubleFunction.cpp
        src/java/util/function/ToDoubleFunction.h
        src/java/util/function/ToIntFunction.cpp
        src/java/util/function/ToIntFunction.h
        src/java/util/function/ToLongFunction.cpp
        src/java/util/function/ToLongFunction.h
        src/java/util/function/ObjDoubleConsumer.cpp
        src/java/util/function/ObjDoubleConsumer.h
        src/java/util/function/DoubleFunction.cpp
        src/java/util/function/DoubleFunction.h
        src/java/util/function/BiPredicate.cpp
        src/java/util/function/BiPredicate.h
        src/java/util/function/BooleanSupplier.cpp
        src/java/util/function/BooleanSupplier.h
        src/java/util/function/DoubleBinaryOperator.cpp
        src/java/util/function/DoubleBinaryOperator.h
        src/java/util/function/DoubleToIntFunction.cpp
        src/java/util/function/DoubleToIntFunction.h
        src/java/util/function/DoubleToLongFunction.cpp
        src/java/util/function/DoubleToLongFunction.h
        src/java/util/function/DoubleUnaryOperator.cpp
        src/java/util/function/DoubleUnaryOperator.h
        src/java/util/AbstractCollection.cpp
        src/java/util/AbstractCollection.h
        src/java/util/AbstractList.cpp
        src/java/util/AbstractList.h
        src/java/util/ArrayList.cpp
        src/java/util/ArrayList.h
        src/Cloneable.h
        src/java/util/AbstractSequentialList.cpp
        src/java/util/AbstractSequentialList.h
        src/java/util/Queue.h
        src/java/util/Deque.h
        src/java/util/LinkedList.cpp
        src/java/util/LinkedList.h
        src/java/util/AbstractSet.cpp
        src/java/util/AbstractSet.h
        src/java/util/Map.cpp
        src/java/util/Map.h
        src/impl_inner/UnmodifiableMap.cpp
        src/impl_inner/UnmodifiableMap.h
        src/impl_inner/SwissTableHashMap.cpp
        src/impl_inner/SwissTableHashMap.h
        src/impl_inner/LeakTrace.h
        src/impl_inner/LeakTrace.cpp
        src/impl_inner/SwissTableHashSet.cpp
        src/impl_inner/SwissTableHashSet.h
        src/impl_inner/HashTableCommon.h
        src/java/util/HashSet.cpp
        src/java/util/HashSet.h
        src/ThreadLocal.cpp
        src/ThreadLocal.h
        src/impl_inner/streams/pipeline/PipelineSpliteratorSource.cpp
        src/impl_inner/streams/pipeline/PipelineSpliteratorSource.h
        src/impl_inner/streams/pipeline/AbstractPipeline.cpp
        src/impl_inner/streams/pipeline/AbstractPipeline.h
        src/impl_inner/streams/pipeline/PipelineStreamAdaptor.cpp
        src/impl_inner/streams/pipeline/PipelineStreamAdaptor.h
        src/impl_inner/streams/pipeline/PipelineDistinctEntries.cpp
        src/impl_inner/streams/pipeline/PipelineDistinctEntries.h
        src/impl_inner/streams/pipeline/PipelineFilterEntries.cpp
        src/impl_inner/streams/pipeline/PipelineFilterEntries.h
        src/impl_inner/streams/pipeline/PipelineFlatMap.cpp
        src/impl_inner/streams/pipeline/PipelineFlatMap.h
        src/impl_inner/streams/pipeline/PipelineLimit.cpp
        src/impl_inner/streams/pipeline/PipelineLimit.h
        src/impl_inner/streams/pipeline/PipelineMap.cpp
        src/impl_inner/streams/pipeline/PipelineMap.h
        src/impl_inner/streams/pipeline/PipelinePeek.cpp
        src/impl_inner/streams/pipeline/PipelinePeek.h
        src/impl_inner/streams/pipeline/PipelineSkip.cpp
        src/impl_inner/streams/pipeline/PipelineSkip.h
        src/impl_inner/streams/pipeline/PipelineArrayListSource.cpp
        src/impl_inner/streams/pipeline/PipelineArrayListSource.h
        src/Integer.cpp
        src/Integer.h
        src/Number.cpp
        src/Number.h
        src/java/util/function/ObjIntConsumer.cpp
        src/java/util/function/ObjIntConsumer.h
        src/java/util/OptionalInt.cpp
        src/java/util/OptionalInt.h
        src/java/util/function/IntSupplier.cpp
        src/java/util/function/IntSupplier.h
        src/java/util/function/IntUnaryOperator.cpp
        src/java/util/function/IntUnaryOperator.h
        src/java/util/function/IntToDoubleFunction.cpp
        src/java/util/function/IntToDoubleFunction.h
        src/java/util/function/IntToLongFunction.cpp
        src/java/util/function/IntToLongFunction.h
        src/java/util/function/IntBinaryOperator.cpp
        src/java/util/function/IntBinaryOperator.h
        src/java/util/IntSummaryStatistics.cpp
        src/java/util/IntSummaryStatistics.h
)
target_include_directories(javapp PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_include_directories(javapp PUBLIC ${CMAKE_SOURCE_DIR}/src/impl_inner)