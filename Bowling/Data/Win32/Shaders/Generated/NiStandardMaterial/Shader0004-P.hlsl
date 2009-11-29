#line 2 "c:\Documents and Settings\Tam\Mis documentos\Documentos (oct09)\school\senior\cis 564\Bowling\Bowling Game\Bowling\Data\Win32\Shaders\Generated\NiStandardMaterial\Shader0004-P.hlsl"
/*
Shader description:
APPLYMODE = 1
WORLDPOSITION = 0
WORLDNORMAL = 0
WORLDNBT = 0
WORLDVIEW = 0
NORMALMAPTYPE = 0
PARALLAXMAPCOUNT = 0
BASEMAPCOUNT = 1
NORMALMAPCOUNT = 0
DARKMAPCOUNT = 0
DETAILMAPCOUNT = 0
BUMPMAPCOUNT = 0
GLOSSMAPCOUNT = 0
GLOWMAPCOUNT = 0
CUSTOMMAP00COUNT = 0
CUSTOMMAP01COUNT = 0
CUSTOMMAP02COUNT = 0
CUSTOMMAP03COUNT = 0
CUSTOMMAP04COUNT = 0
DECALMAPCOUNT = 0
FOGENABLED = 0
ENVMAPTYPE = 0
PROJLIGHTMAPCOUNT = 0
PROJLIGHTMAPTYPES = 0
PROJLIGHTMAPCLIPPED = 0
PROJSHADOWMAPCOUNT = 0
PROJSHADOWMAPTYPES = 0
PROJSHADOWMAPCLIPPED = 0
PERVERTEXLIGHTING = 1
UVSETFORMAP00 = 0
UVSETFORMAP01 = 0
UVSETFORMAP02 = 0
UVSETFORMAP03 = 0
UVSETFORMAP04 = 0
UVSETFORMAP05 = 0
UVSETFORMAP06 = 0
UVSETFORMAP07 = 0
UVSETFORMAP08 = 0
UVSETFORMAP09 = 0
UVSETFORMAP10 = 0
UVSETFORMAP11 = 0
POINTLIGHTCOUNT = 0
SPOTLIGHTCOUNT = 0
DIRLIGHTCOUNT = 0
SHADOWMAPFORLIGHT = 0
SPECULAR = 0
AMBDIFFEMISSIVE = 0
LIGHTINGMODE = 0
APPLYAMBIENT = 0
BASEMAPALPHAONLY = 0
APPLYEMISSIVE = 0
SHADOWTECHNIQUE = 0
PSSMSLICETRANSITIONSENABLED = 0
PSSMSLICECOUNT = 0
PSSMWHICHLIGHT = 0
ALPHATEST = 0
*/

//---------------------------------------------------------------------------
// Types:
#if defined(DIRECT3D)
#define BONEMATRIX_TYPE float4x3
#else
#define BONEMATRIX_TYPE float3x4
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant variables:
//---------------------------------------------------------------------------

sampler2D Base;
//---------------------------------------------------------------------------
// Functions:
//---------------------------------------------------------------------------

/*

    Separate a float4 into a float3 and a float.   
    
*/

void SplitColorAndOpacity(float4 ColorAndOpacity,
    out float3 Color,
    out float Opacity)
{

    Color.rgb = ColorAndOpacity.rgb;
    Opacity = ColorAndOpacity.a;
    
}
//---------------------------------------------------------------------------
/*

    This fragment is responsible for sampling a texture and returning its value
    as a RGB value and an A value.
    
*/

void TextureRGBASample(float2 TexCoord,
    sampler2D Sampler,
    bool Saturate,
    out float4 ColorOut)
{

    ColorOut = tex2D(Sampler, TexCoord);
    if (Saturate)
    {
        ColorOut = saturate(ColorOut);
    }
    
}
//---------------------------------------------------------------------------
/*

    This fragment is responsible for multiplying two float3's. 
    
*/

void MultiplyFloat3(float3 V1,
    float3 V2,
    out float3 Output)
{

    Output = V1 * V2;
    
}
//---------------------------------------------------------------------------
/*

    This fragment is responsible for multiplying two floats. 
    
*/

void MultiplyFloat(float V1,
    float V2,
    out float Output)
{

    Output = V1 * V2;
    
}
//---------------------------------------------------------------------------
/*

    This fragment is responsible for computing the final RGB color.
    
*/

void CompositeFinalRGBColor(float3 DiffuseColor,
    float3 SpecularColor,
    out float3 OutputColor)
{

    OutputColor.rgb = DiffuseColor.rgb + SpecularColor.rgb;
    
}
//---------------------------------------------------------------------------
/*

    This fragment is responsible for computing the final RGBA color.
    
*/

void CompositeFinalRGBAColor(float3 FinalColor,
    float FinalOpacity,
    out float4 OutputColor)
{

    OutputColor.rgb = FinalColor.rgb;
    OutputColor.a = saturate(FinalOpacity);
    
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Input:
//---------------------------------------------------------------------------

struct Input
{
    float4 PosProjected : POSITION0;
    float4 DiffuseAccum : TEXCOORD0;
    float2 UVSet0 : TEXCOORD1;

};

//---------------------------------------------------------------------------
// Output:
//---------------------------------------------------------------------------

struct Output
{
    float4 Color0 : COLOR0;

};

//---------------------------------------------------------------------------
// Main():
//---------------------------------------------------------------------------

Output Main(Input In)
{
    Output Out;
	// Function call #0
    float3 Color_CallOut0;
    float Opacity_CallOut0;
    SplitColorAndOpacity(In.DiffuseAccum, Color_CallOut0, Opacity_CallOut0);

	// Function call #1
    float4 ColorOut_CallOut1;
    TextureRGBASample(In.UVSet0, Base, bool(false), ColorOut_CallOut1);

	// Function call #2
    float3 Color_CallOut2;
    float Opacity_CallOut2;
    SplitColorAndOpacity(ColorOut_CallOut1, Color_CallOut2, Opacity_CallOut2);

	// Function call #3
    float3 Output_CallOut3;
    MultiplyFloat3(Color_CallOut0, Color_CallOut2, Output_CallOut3);

	// Function call #4
    float Output_CallOut4;
    MultiplyFloat(Opacity_CallOut0, Opacity_CallOut2, Output_CallOut4);

	// Function call #5
    float3 OutputColor_CallOut5;
    CompositeFinalRGBColor(Output_CallOut3, float3(0.0, 0.0, 0.0), 
        OutputColor_CallOut5);

	// Function call #6
    CompositeFinalRGBAColor(OutputColor_CallOut5, Output_CallOut4, Out.Color0);

    return Out;
}

