TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -L$$PWD/bin/ -lfbxsdk -ldl

SOURCES += main.cpp \
    myparser.cpp \
    MyModelFormat/MyInternalFormat/mynode.cpp \
    MyModelFormat/MyInternalFormat/mynodeanimation.cpp \
    MyModelFormat/MyInternalFormat/myanimation.cpp \
    MyModelFormat/mymodel.cpp \
    MyModelFormat/myexporter.cpp \
    MyModelFormat/myimporter.cpp \
    MyModelFormat/MyInternalFormat/mystack.cpp

HEADERS += \
    myparser.h \
    MyModelFormat/MyInternalFormat/mynode.h \
    -L/extern \
    extern/glm/detail/_features.hpp \
    extern/glm/detail/_fixes.hpp \
    extern/glm/detail/_noise.hpp \
    extern/glm/detail/_swizzle.hpp \
    extern/glm/detail/_swizzle_func.hpp \
    extern/glm/detail/_vectorize.hpp \
    extern/glm/detail/func_common.hpp \
    extern/glm/detail/func_exponential.hpp \
    extern/glm/detail/func_geometric.hpp \
    extern/glm/detail/func_integer.hpp \
    extern/glm/detail/func_matrix.hpp \
    extern/glm/detail/func_packing.hpp \
    extern/glm/detail/func_trigonometric.hpp \
    extern/glm/detail/func_vector_relational.hpp \
    extern/glm/detail/intrinsic_common.hpp \
    extern/glm/detail/intrinsic_exponential.hpp \
    extern/glm/detail/intrinsic_geometric.hpp \
    extern/glm/detail/intrinsic_integer.hpp \
    extern/glm/detail/intrinsic_matrix.hpp \
    extern/glm/detail/intrinsic_trigonometric.hpp \
    extern/glm/detail/intrinsic_vector_relational.hpp \
    extern/glm/detail/precision.hpp \
    extern/glm/detail/setup.hpp \
    extern/glm/detail/type_float.hpp \
    extern/glm/detail/type_gentype.hpp \
    extern/glm/detail/type_half.hpp \
    extern/glm/detail/type_int.hpp \
    extern/glm/detail/type_mat.hpp \
    extern/glm/detail/type_mat2x2.hpp \
    extern/glm/detail/type_mat2x3.hpp \
    extern/glm/detail/type_mat2x4.hpp \
    extern/glm/detail/type_mat3x2.hpp \
    extern/glm/detail/type_mat3x3.hpp \
    extern/glm/detail/type_mat3x4.hpp \
    extern/glm/detail/type_mat4x2.hpp \
    extern/glm/detail/type_mat4x3.hpp \
    extern/glm/detail/type_mat4x4.hpp \
    extern/glm/detail/type_vec.hpp \
    extern/glm/detail/type_vec1.hpp \
    extern/glm/detail/type_vec2.hpp \
    extern/glm/detail/type_vec3.hpp \
    extern/glm/detail/type_vec4.hpp \
    extern/glm/gtc/bitfield.hpp \
    extern/glm/gtc/color_space.hpp \
    extern/glm/gtc/constants.hpp \
    extern/glm/gtc/epsilon.hpp \
    extern/glm/gtc/integer.hpp \
    extern/glm/gtc/matrix_access.hpp \
    extern/glm/gtc/matrix_integer.hpp \
    extern/glm/gtc/matrix_inverse.hpp \
    extern/glm/gtc/matrix_transform.hpp \
    extern/glm/gtc/noise.hpp \
    extern/glm/gtc/packing.hpp \
    extern/glm/gtc/quaternion.hpp \
    extern/glm/gtc/random.hpp \
    extern/glm/gtc/reciprocal.hpp \
    extern/glm/gtc/round.hpp \
    extern/glm/gtc/type_precision.hpp \
    extern/glm/gtc/type_ptr.hpp \
    extern/glm/gtc/ulp.hpp \
    extern/glm/gtc/vec1.hpp \
    extern/glm/gtx/associated_min_max.hpp \
    extern/glm/gtx/bit.hpp \
    extern/glm/gtx/closest_point.hpp \
    extern/glm/gtx/color_space.hpp \
    extern/glm/gtx/color_space_YCoCg.hpp \
    extern/glm/gtx/common.hpp \
    extern/glm/gtx/compatibility.hpp \
    extern/glm/gtx/component_wise.hpp \
    extern/glm/gtx/dual_quaternion.hpp \
    extern/glm/gtx/euler_angles.hpp \
    extern/glm/gtx/extend.hpp \
    extern/glm/gtx/extented_min_max.hpp \
    extern/glm/gtx/fast_exponential.hpp \
    extern/glm/gtx/fast_square_root.hpp \
    extern/glm/gtx/fast_trigonometry.hpp \
    extern/glm/gtx/gradient_paint.hpp \
    extern/glm/gtx/handed_coordinate_space.hpp \
    extern/glm/gtx/hash.hpp \
    extern/glm/gtx/integer.hpp \
    extern/glm/gtx/intersect.hpp \
    extern/glm/gtx/io.hpp \
    extern/glm/gtx/log_base.hpp \
    extern/glm/gtx/matrix_cross_product.hpp \
    extern/glm/gtx/matrix_decompose.hpp \
    extern/glm/gtx/matrix_interpolation.hpp \
    extern/glm/gtx/matrix_major_storage.hpp \
    extern/glm/gtx/matrix_operation.hpp \
    extern/glm/gtx/matrix_query.hpp \
    extern/glm/gtx/matrix_transform_2d.hpp \
    extern/glm/gtx/mixed_product.hpp \
    extern/glm/gtx/norm.hpp \
    extern/glm/gtx/normal.hpp \
    extern/glm/gtx/normalize_dot.hpp \
    extern/glm/gtx/number_precision.hpp \
    extern/glm/gtx/optimum_pow.hpp \
    extern/glm/gtx/orthonormalize.hpp \
    extern/glm/gtx/perpendicular.hpp \
    extern/glm/gtx/polar_coordinates.hpp \
    extern/glm/gtx/projection.hpp \
    extern/glm/gtx/quaternion.hpp \
    extern/glm/gtx/range.hpp \
    extern/glm/gtx/raw_data.hpp \
    extern/glm/gtx/rotate_normalized_axis.hpp \
    extern/glm/gtx/rotate_vector.hpp \
    extern/glm/gtx/scalar_multiplication.hpp \
    extern/glm/gtx/scalar_relational.hpp \
    extern/glm/gtx/simd_mat4.hpp \
    extern/glm/gtx/simd_quat.hpp \
    extern/glm/gtx/simd_vec4.hpp \
    extern/glm/gtx/spline.hpp \
    extern/glm/gtx/std_based_type.hpp \
    extern/glm/gtx/string_cast.hpp \
    extern/glm/gtx/transform.hpp \
    extern/glm/gtx/transform2.hpp \
    extern/glm/gtx/type_aligned.hpp \
    extern/glm/gtx/vector_angle.hpp \
    extern/glm/gtx/vector_query.hpp \
    extern/glm/gtx/wrap.hpp \
    extern/glm/common.hpp \
    extern/glm/exponential.hpp \
    extern/glm/ext.hpp \
    extern/glm/fwd.hpp \
    extern/glm/geometric.hpp \
    extern/glm/glm.hpp \
    extern/glm/integer.hpp \
    extern/glm/mat2x2.hpp \
    extern/glm/mat2x3.hpp \
    extern/glm/mat2x4.hpp \
    extern/glm/mat3x2.hpp \
    extern/glm/mat3x3.hpp \
    extern/glm/mat3x4.hpp \
    extern/glm/mat4x2.hpp \
    extern/glm/mat4x3.hpp \
    extern/glm/mat4x4.hpp \
    extern/glm/matrix.hpp \
    extern/glm/packing.hpp \
    extern/glm/trigonometric.hpp \
    extern/glm/vec2.hpp \
    extern/glm/vec3.hpp \
    extern/glm/vec4.hpp \
    extern/glm/vector_relational.hpp \
    Common/vab.h \
    MyModelFormat/MyInternalFormat/mynodeanimation.h \
    MyModelFormat/MyInternalFormat/myanimation.h \
    MyModelFormat/MyInternalFormat/myfiletype.h \
    MyModelFormat/mymodel.h \
    MyModelFormat/myexporter.h \
    MyModelFormat/myimporter.h \
    MyModelFormat/MyInternalFormat/myidentifier.h \
    MyModelFormat/MyInternalFormat/mystack.h

DISTFILES += \
    data/Test.png \
    data/Test.fbx \
    data/TestBin.fbx
