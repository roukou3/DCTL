
#define PI (3.14159265)

__DEVICE__ float radian(const float degree){
  const float _t = 180;
  return _fdivide(degree, _t) * PI;
}

__DEVICE__ float degree(const float radian){
  const float _t = PI;
  return _fdivide(radian, _t) * 180;
}

__DEVICE__ float2 rotate2(const float2 vector, const float angle_rad){
  float2 out;
  const float cos = _cosf(angle_rad);
  const float sin = _sinf(angle_rad);
  out.x = _fmaf(vector.x, cos, - vector.y * sin);
  out.y = _fmaf(vector.x, sin,   vector.y * cos);
  return out;
}

__DEVICE__ float3 pan_tilt_rotate(float3 xyz_in, float3 angles_in_degree){
  float3 xyz = xyz_in;
  float2 temp;
  temp = rotate2(make_float2(xyz.x, xyz.y), radian(angles_in_degree.z));//rotate
  xyz.x = temp.x;
  xyz.y = temp.y;
  temp = rotate2(make_float2(xyz.y, xyz.z), radian(angles_in_degree.y));//tilt
  xyz.y = temp.x;
  xyz.z = temp.y;
  temp = rotate2(make_float2(xyz.x, xyz.z), radian(angles_in_degree.x));//pan
  xyz.x = temp.x;
  xyz.z = temp.y;
  return xyz;
}

__DEVICE__ float bicubic_h(const float t){
  const float x = _fabs(t);
  if(x <= 1){
    return (x-1)*(x*(x-1)-1);
  }else if(x <= 2){
    return -(x-1)*(x-2)*(x-2);
  }
  return 0.0;
}

__DEVICE__ float _tex2D_bicubic(const __TEXTURE__ p_Tex, const float pX, const float pY){
  const float iX = _floor(pX);
  const float iY = _floor(pY);
  const float dX = pX - iX;
  const float dY = pY - iY;
  const float vecX[] = {1 + dX, dX, 1 - dX, 2 - dX};
  const float vecY[] = {1 + dY, dY, 1 - dY, 2 - dY};

  float sum = 0;
  float coef = 0;
  for(int i = 0; i < 4; i++){
    float hX = bicubic_h(vecX[i]);
    for(int j = 0; j < 4; j++){
      float hY = bicubic_h(vecY[j]);
      sum += hY * _tex2D(p_Tex, iX + i - 1, iY + j -1) * hX;
      coef += hY * hX;
    }
  }
  return _fdivide(sum, coef);
}

__DEVICE__ float _tex2D_bilinear(const __TEXTURE__ p_Tex, const float pX, const float pY){
  const float iX = _floor(pX);
  const float iY = _floor(pY);
  const float dX = pX - iX;
  const float dY = pY - iY;
  const float vecX[] = {1 - dX, dX};
  const float vecY[] = {1 - dY, dY};

  float sum = 0;
  for(int i = 0; i < 2; i++){
    for(int j = 0; j < 2; j++){
      sum += vecY[j] * _tex2D(p_Tex, iX+i, iY+j) * vecX[i];
    }
  }
  return sum;
}

__DEVICE__ float3 text2D_bicubicRGB(const float pX, const float pY, __TEXTURE__ p_TexR, __TEXTURE__ p_TexG, __TEXTURE__ p_TexB){
  float3 rgb;
  rgb.x = _tex2D_bicubic(p_TexR, pX, pY);
  rgb.y = _tex2D_bicubic(p_TexG, pX, pY);
  rgb.z = _tex2D_bicubic(p_TexB, pX, pY);
  return rgb;
}

__DEVICE__ float3 text2D_RGB(const float pX, const float pY, __TEXTURE__ p_TexR, __TEXTURE__ p_TexG, __TEXTURE__ p_TexB){
  float3 rgb;
  rgb.x = _tex2D(p_TexR, _round(pX), _round(pY));
  rgb.y = _tex2D(p_TexG, _round(pX), _round(pY));
  rgb.z = _tex2D(p_TexB, _round(pX), _round(pY));
  return rgb;
}

__DEVICE__ float3 equirect_angel2space(const float2 eq_thetaphi){
  const float theta = eq_thetaphi.x;
  const float phi   = eq_thetaphi.y;
  float3 xyz;
  xyz.z = _sinf(theta) * _cosf(phi);
  xyz.x = - _sinf(theta) * _sinf(phi);
  xyz.y = - _cosf(theta);
  return xyz;
}

__DEVICE__ float2 space2angle(const float3 xyz){
  float2 thetaphi;
  const float _z = _hypotf(_hypotf(xyz.x, xyz.y), xyz.z);
  const float theta = _acosf(_fdivide(xyz.z, _z));
  const float _x = _hypotf(xyz.x, xyz.y);
  const float _cn = -1;
  const float _cp =  1;
  const float phi = _copysignf(_acosf(_clampf(_fdivide(xyz.x, _x), _cn, _cp)), xyz.y);
  thetaphi.x = theta;
  thetaphi.y = phi;
  return thetaphi;
}

