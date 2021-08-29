#version 460
layout (location = 0) out vec3 outColor;

layout(set = 0, binding = 0) uniform CameraBuffer{
    mat4 view;
    mat4 proj;
    mat4 viewproj;
} cameraData;

//push constants block
layout( push_constant ) uniform constants
{
    vec4 points[2];
    vec4 color;
} PushConstants;

void main()
{
    mat4 modelMatrix = mat4(1.0);
    modelMatrix[0][0] = PushConstants.points[gl_VertexIndex][0];
    modelMatrix[1][1] = PushConstants.points[gl_VertexIndex][1];
    modelMatrix[2][2] = PushConstants.points[gl_VertexIndex][2];

    mat4 transformMatrix = (cameraData.viewproj * modelMatrix);
    gl_Position = transformMatrix * vec4(1.0f);
    outColor = PushConstants.color.xyz;
}
