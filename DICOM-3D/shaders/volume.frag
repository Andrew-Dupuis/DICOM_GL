#version 300 es
precision highp int;
precision highp float;					

uniform highp sampler3D volume;
// WebGL doesn't support 1D textures, so we use a 2D texture for the transfer function
uniform highp sampler2D colormap;
uniform int useColormaps;
uniform int window;
uniform int level;

uniform ivec3 volume_dims;

in vec3 eyeDirection;
flat in vec3 eyePosition;

out vec4 color;

float brightness = 100.0;

vec2 intersect_box(vec3 orig, vec3 dir) {
	const vec3 box_min = vec3(0);
	const vec3 box_max = vec3(1);
	vec3 inv_dir = 1.0 / dir;
	vec3 tmin_tmp = (box_min - orig) * inv_dir;
	vec3 tmax_tmp = (box_max - orig) * inv_dir;
	vec3 tmin = min(tmin_tmp, tmax_tmp);
	vec3 tmax = max(tmin_tmp, tmax_tmp);
	float t0 = max(tmin.x, max(tmin.y, tmin.z));
	float t1 = min(tmax.x, min(tmax.y, tmax.z));
	return vec2(t0, t1);
}

void main(void) {
	vec3 ray_dir = normalize(eyeDirection);
	vec2 t_hit = intersect_box(eyePosition, ray_dir);
	if (t_hit.x > t_hit.y) {
		discard;
	}
	t_hit.x = max(t_hit.x, 0.0);
	vec3 dt_vec = 1.0 / (vec3(volume_dims) * abs(ray_dir));
	float dt = min(dt_vec.x, min(dt_vec.y, dt_vec.z));
	vec3 p = eyePosition + t_hit.x * ray_dir;
	for (float t = t_hit.x; t < t_hit.y; t += dt) {
		int value = int(texture(volume, p).r * float(65535));
    		value = (value - level);
    		float valScaled = (float(value)/float(2*window))+0.5;
		vec4 val_color;
   		if(useColormaps != 1){
    		val_color= vec4(valScaled,valScaled,valScaled,valScaled);
    	}
    	else{
			val_color= vec4(texture(colormap, vec2(valScaled,0.5)).rgb,valScaled);
   		}
		if(valScaled>0.01){
			color.rgb += (1.0 - color.a) * val_color.a * val_color.rgb;
			color.a += (1.0 - color.a) * val_color.a;
		}

		p += ray_dir * dt;
	}
}