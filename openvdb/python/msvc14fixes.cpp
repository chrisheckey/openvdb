// Linker fix for msvc14 update 3: missing reference(s) to get_pointer()
// boost::python bug report
// https://github.com/boostorg/python/issues/116
// https://stackoverflow.com/questions/38261530/unresolved-external-symbols-since-visual-studio-2015-update-3-boost-python-link
// http://cegui.org.uk/forum/viewtopic.php?t=7276

/*
Visual C++: Passing non-pointer-like types to uninitialized_copy, uninitialized_copy_n, or uninitialized_fill

Issue:
Passing non-pointer-like types to uninitialized_copy, uninitialized_copy_n, or uninitialized_fill fails to compile.
Workaround:
Provide a pointer_traits specialization for the supplied type.
*/


// msvc14 update 3
#if ( defined(_MSC_VER) && (_MSC_VER > 1800) && (_MSC_FULL_VER > 190023918) )

#include <boost/get_pointer.hpp>
#include <openvdb/Grid.h>
// #include <openvdb/Metadata.h>

// #include <memory>  // std::pointer_traits

#define DEFINE_GET_POINTER_MSVC14( X )                                            \
namespace boost                                                                   \
{                                                                                 \
    template <>                                                                   \
    X const volatile * get_pointer<X const volatile >( X const volatile *c )      \
    {                                                                             \
        return c;                                                                 \
    }                                                                             \
}																			

#define COMMA , 

DEFINE_GET_POINTER_MSVC14(
	class openvdb::OPENVDB_VERSION_NAME::Grid<
	class openvdb::OPENVDB_VERSION_NAME::tree::Tree<
	class openvdb::OPENVDB_VERSION_NAME::tree::RootNode<
	class openvdb::OPENVDB_VERSION_NAME::tree::InternalNode<
	class openvdb::OPENVDB_VERSION_NAME::tree::InternalNode<
	class openvdb::OPENVDB_VERSION_NAME::tree::LeafNode<
	float COMMA 3>
	COMMA 4>
	COMMA 5>
	>
	>
	>
)

DEFINE_GET_POINTER_MSVC14(
	class openvdb::OPENVDB_VERSION_NAME::Grid<
	class openvdb::OPENVDB_VERSION_NAME::tree::Tree<
	class openvdb::OPENVDB_VERSION_NAME::tree::RootNode<
	class openvdb::OPENVDB_VERSION_NAME::tree::InternalNode<
	class openvdb::OPENVDB_VERSION_NAME::tree::InternalNode<
	class openvdb::OPENVDB_VERSION_NAME::tree::LeafNode<
	class openvdb::OPENVDB_VERSION_NAME::math::Vec3<
	float>
	COMMA 3>
	COMMA 4>
	COMMA 5>
	>
	>
	>
)

DEFINE_GET_POINTER_MSVC14(
	class openvdb::OPENVDB_VERSION_NAME::Grid<
	class openvdb::OPENVDB_VERSION_NAME::tree::Tree<
	class openvdb::OPENVDB_VERSION_NAME::tree::RootNode<
	class openvdb::OPENVDB_VERSION_NAME::tree::InternalNode<
	class openvdb::OPENVDB_VERSION_NAME::tree::InternalNode<
	class openvdb::OPENVDB_VERSION_NAME::tree::LeafNode<
	bool COMMA 3>
	COMMA 4>
	COMMA 5>
	>
	>
	>
)

// Does not work for Abstract Class
//DEFINE_GET_POINTER_MSVC14(class openvdb::OPENVDB_VERSION_NAME::Metadata)

#endif // msvc14 update 3
