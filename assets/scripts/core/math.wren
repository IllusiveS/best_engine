foreign class Vector3 {
    construct new (_x, _y, _z) {
        x = _x
        y = _y
        z = _z
    }

    foreign normalize()
    foreign magnitude()
    foreign add(vec)

    //accessors
    foreign x
    foreign x=( rhs )
    foreign y
    foreign y=( rhs )
    foreign z
    foreign z=( rhs )
}

