//
// Created by patwys on 5/9/2021.
//

#include "Transform.h"

void Vector3::bind_methods(wrenpp::VM& vm) {
	vm.beginModule("assets/scripts/core/math")
		.bindClass<Vector3, float, float, float>("Vector3")
			.bindGetter< decltype(Vector3::x), &Vector3::x >( "x" )
			.bindSetter< decltype(Vector3::x), &Vector3::x >( "x=(_)" )
			.bindGetter< decltype(Vector3::y), &Vector3::y >( "y" )
			.bindSetter< decltype(Vector3::y), &Vector3::y >( "y=(_)" )
			.bindGetter< decltype(Vector3::z), &Vector3::z >( "z" )
			.bindSetter< decltype(Vector3::z), &Vector3::z >( "z=(_)" )
	        .bindMethod<decltype(&Vector3::normalize), &Vector3::normalize>(false, "normalize()")
			.bindMethod<decltype(&Vector3::magnitude), &Vector3::magnitude>(false, "magnitude()")
			.bindMethod<decltype(&Vector3::add), &Vector3::add>(false, "add(_)")
        .endClass()
    .endModule();
}
