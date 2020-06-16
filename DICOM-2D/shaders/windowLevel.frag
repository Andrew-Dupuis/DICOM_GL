#version 300 es
precision highp int;
precision highp float;					

uniform highp sampler2D tex;
uniform highp sampler2D colormap;

uniform int useColormaps;
uniform int window;
uniform int level;

in vec2 uvFrag;
out vec4 color;

void main(void) {
    int value = int(texture(tex, uvFrag).r * float(65535));
    value = (value - level);
    float valScaled = (float(value)/float(2*window))+0.5;
    if(useColormaps != 1){
    	color = vec4(valScaled,valScaled,valScaled,1);
    }
    else{
	    color = texture(colormap, vec2(valScaled,0.5));
    }
}