<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<shader id = "default/unlit_transparent">
	<pass src = "data/shader/unlit.hlsl">
		<vert entry = "Vert" />
		<pixel entry = "Pixel" />

		<!--depth will use "always" and write = "false" by default to maintain 2D functionality-->
		<!--That is effectively saying depth is disabled-->
		<!-- for 3D, you want to set "write" to true and test to "lequal" -->
		<!--test:  test: never, lt, eq, leq, gt, geq, neq, always-->
		<depth write = "true" comp = "geq" />
		<blend mode="transparent" />

		<!--OPTIONAL EXTRA::RasterState-->
		<!--cull: back, front, none-->
		<!--frontface: ccw(true), cw(false)-->
		<!--fill: solid, wire-->
		<raster cull = "none" frontCCW = "true" fill = "solid"/>
	</pass>
</shader>