#define MODEL_INSTANCE_MAX_COUNT 64

// --
// struct
// --

struct KeyframeDesc
{
	int Clip;
	float Time;
	float2 Padding;
};

struct KeyframeCount
{
	uint Translation;
	uint Rotaion;
	uint Scale;
	uint MaxCount;
};

struct Transform
{
	float3 Translation;
	float4 Rotaion;
	float3 Scale;
};

// --
// buffer
// --

Texture2DArray InputClipMap;
StructuredBuffer<KeyframeCount> InputKeyframeCount;


// --
// function
// --

matrix Combine(Transform transform)
{
	float3 t = transform.Translation;
	float4 r = transform.Rotaion;
	float3 s = transform.Scale;

	float xz2 = 2 * r.x * r.z;
	float yz2 = 2 * r.y * r.z;
	float xy2 = 2 * r.x * r.y;

	float xx2 = 2 * r.x * r.x;
	float yy2 = 2 * r.y * r.y;
	float zz2 = 2 * r.z * r.z;

	float xw2 = 2 * r.x * r.w;
	float yw2 = 2 * r.y * r.w;
	float zw2 = 2 * r.z * r.w;

	return matrix(
		(xz2 + yw2) * s.x, (yz2 - xw2) * s.x, (1.0f - xx2 - yy2) * s.x, 0.0f,
		(xy2 - zw2) * s.y, (1.0f - xx2 - zz2) * s.y, (yz2 + xw2) * s.y, 0.0f,
		(1.0f - yy2 - zz2) * s.z, (xy2 + zw2) * s.z, (xz2 - yw2) * s.z, 0.0f,
		t.x, t.y, t.z, 1.0f
		);
}

float4 Slerp(float4 r1, float4 r2, float t)
{
	float rad = acos(dot(r1, r2));
	return (sin((1 - t) * rad) * r1 + sin(t * rad) * r2) / sin(rad);
}

void Lerp(out Transform tOut, Transform t1, Transform t2, float t)
{
	tOut.Translation = lerp(t1.Translation, t2.Translation, t);
	tOut.Rotaion = Slerp(t1.Rotaion, t2.Rotaion, t);
	tOut.Scale = lerp(t1.Scale, t2.Scale, t);
}

struct AnimTime
{
	int currT, currR, currS;
	float timeT, timeR, timeS;
};

void GetAnimTime(out AnimTime a, uint boneIndex, int clip, float time)
{
	a.currT = a.currR = a.currS = -1;
	a.timeT = a.timeR = a.timeS = 0.0f;

	uint tCount = InputKeyframeCount[boneIndex].Translation;
	uint rCount = InputKeyframeCount[boneIndex].Rotaion;
	uint sCount = InputKeyframeCount[boneIndex].Scale;
	uint maxCount = InputKeyframeCount[boneIndex].MaxCount;

	float3 prvFrame = float3(0, 0, 0);
	for (uint i = 0; i < maxCount; i++)
	{
		float3 frame = InputClipMap.Load(int4(boneIndex * 4 + 3, i, clip, 0)).xyz;

		bool bT, bR, bS;
		bT = bR = bS = false;

		if (a.currT == -1)
		{
			if (i >= tCount)
			{
				a.currT = tCount - 1;
				a.timeT = 0.0f;
				bT = true;
			}
			else if (time <= frame.x)
			{
				a.currT = i;
				float dist = frame.x - prvFrame.x;

				if (dist != 0)
				{
					a.timeT = (time - prvFrame.x) / dist;
				}
				else
				{
					a.timeT = 0.0f;
				}
				bT = true;
			}
		}
		else bT = true;
		if (a.currR == -1)
		{
			if (i >= rCount)
			{
				a.currR = rCount - 1;
				a.timeR = 0.0f;
				bR = true;
			}
			else if (time <= frame.y)
			{
				a.currR = i;
				float dist = frame.y - prvFrame.y;

				if (dist != 0)
				{
					a.timeR = (time - prvFrame.y) / dist;
				}
				else
				{
					a.timeR = 0.0f;
				}
				bR = true;
			}
		}
		else bR = true;
		if (a.currS == -1)
		{
			if (i >= sCount)
			{
				a.currS = sCount - 1;
				a.timeS = 0.0f;
				bS = true;
			}
			else if (time <= frame.z)
			{
				a.currS = i;
				float dist = frame.z - prvFrame.z;

				if (dist != 0)
				{
					a.timeS = (time - prvFrame.z) / dist;
				}
				else
				{
					a.timeS = 0.0f;
				}
				bS = true;
			}
		}
		else bS = true;

		if (bT && bR && bS) break;
		else prvFrame = frame;
	}
}

void GetAnimWorld(out Transform transform, uint boneIndex, int clip, float time)
{
	transform = (Transform)0;

	[flatten]
	if (clip == -1) return;

	AnimTime aTime;
	GetAnimTime(aTime, boneIndex, clip, time);

	float3 t = InputClipMap.Load(int4(boneIndex * 4 + 0, aTime.currT, clip, 0)).xyz;
	[flatten]
	if (aTime.timeT != 0.0f)
	{
		float3 prev = InputClipMap.Load(int4(boneIndex * 4 + 0, aTime.currT - 1, clip, 0)).xyz;
		t = lerp(prev, t, aTime.timeT);
	}

	float4 r = InputClipMap.Load(int4(boneIndex * 4 + 1, aTime.currR, clip, 0));
	[flatten]
	if (aTime.timeR != 0.0f)
	{
		float4 prev = InputClipMap.Load(int4(boneIndex * 4 + 0, aTime.currT - 1, clip, 0));
		r = Slerp(prev, r, aTime.timeR);
	}

	float3 s = InputClipMap.Load(int4(boneIndex * 4 + 2, aTime.currS, clip, 0)).xyz;
	[flatten]
	if (aTime.timeS != 0.0f)
	{
		float3 prev = InputClipMap.Load(int4(boneIndex * 4 + 0, aTime.currT - 1, clip, 0)).xyz;
		s = lerp(prev, s, aTime.timeS);
	}

	transform.Translation = t;
	transform.Rotaion = r;
	transform.Scale = s;
}