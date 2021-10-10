#include "./Header/DrawPolygon.h"
#include "./Header/Camera.h"

#define EoF -1 // End of Function

DrawPolygon::DrawPolygon(const DirectXInit* w) :
	DebugText(w),
	polygonCount(0),
	loopCount(0),
	lightVec{},
	light(100, -100, 100),
	cameraNo(MAIN_CAMERA)
{
	using DirectX::XMLoadFloat3;

	XMFLOAT3 cameraPos = { 0, 0, -100 };
	XMFLOAT3 cameraTarget = { 0, 0, 0 };
	XMFLOAT3 upVector = { 0, 1, 0 };

	matView.push_back(Camera::CreateCamera(
		XMLoadFloat3(&cameraPos),
		XMLoadFloat3(&cameraTarget),
		XMLoadFloat3(&upVector)
	));
}

int DrawPolygon::CreateTriangle(
	const XMFLOAT3& vertex1, const DirectX::XMFLOAT2& uv1,
	const XMFLOAT3& vertex2, const DirectX::XMFLOAT2& uv2,
	const XMFLOAT3& vertex3, const DirectX::XMFLOAT2& uv3,
	const bool& isFill)
{
	vertices.push_back({});

	vertices[vertices.size() - 1].vertices.push_back({ vertex1, {}, uv1 });
	vertices[vertices.size() - 1].vertices.push_back({ vertex2, {}, uv2 });
	vertices[vertices.size() - 1].vertices.push_back({ vertex3, {}, uv3 });

	if (isFill == true)
	{
		vertices[vertices.size() - 1].indices.push_back(0);
		vertices[vertices.size() - 1].indices.push_back(1);
		vertices[vertices.size() - 1].indices.push_back(2);
	}
	else
	{
		vertices[vertices.size() - 1].indices.push_back(0);
		vertices[vertices.size() - 1].indices.push_back(1);
		vertices[vertices.size() - 1].indices.push_back(1);
		vertices[vertices.size() - 1].indices.push_back(2);
		vertices[vertices.size() - 1].indices.push_back(2);
		vertices[vertices.size() - 1].indices.push_back(0);
	}

	return CreateVertexAndIndexBuffer();
}

int DrawPolygon::CreateRect(const float& width, const float& height, const bool& isFill)
{
	vertices.push_back({});

	vertices[vertices.size() - 1].vertices.push_back({ { -width / 2, +height / 2, 0.0f }, {}, { 0.0f, 0.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { +width / 2, +height / 2, 0.0f }, {}, { 1.0f, 0.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { -width / 2, -height / 2, 0.0f }, {}, { 0.0f, 1.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { +width / 2, +height / 2, 0.0f }, {}, { 1.0f, 0.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { -width / 2, -height / 2, 0.0f }, {}, { 0.0f, 1.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { +width / 2, -height / 2, 0.0f }, {}, { 1.0f, 1.0f } });

	if (isFill == true)
	{
		vertices[vertices.size() - 1].indices.push_back(0);
		vertices[vertices.size() - 1].indices.push_back(1);
		vertices[vertices.size() - 1].indices.push_back(2);
		vertices[vertices.size() - 1].indices.push_back(4);
		vertices[vertices.size() - 1].indices.push_back(3);
		vertices[vertices.size() - 1].indices.push_back(5);
	}
	else
	{
		vertices[vertices.size() - 1].indices.push_back(0);
		vertices[vertices.size() - 1].indices.push_back(1);
		vertices[vertices.size() - 1].indices.push_back(3);
		vertices[vertices.size() - 1].indices.push_back(5);
		vertices[vertices.size() - 1].indices.push_back(5);
		vertices[vertices.size() - 1].indices.push_back(2);
		vertices[vertices.size() - 1].indices.push_back(4);
		vertices[vertices.size() - 1].indices.push_back(0);
	}

	return CreateVertexAndIndexBuffer();
}

int DrawPolygon::CreateCircle(const float& r, const size_t& divNum, const bool& isFill)
{
	vertices.push_back({});

	Vertex* v = (Vertex*)malloc(sizeof(Vertex) * (divNum + 1));
	size_t indexSize = divNum * (2 + (size_t)isFill);
	unsigned short* index = (unsigned short*)malloc(sizeof(unsigned short) * indexSize);

	Circle({ 0, 0, 0 }, r, divNum, isFill, v, index);

	for (size_t i = 0; i < divNum + 1; i++)
	{
		vertices[vertices.size() - 1].vertices.push_back(v[i]);
	}
	for (size_t i = 0; i < indexSize; i++)
	{
		vertices[vertices.size() - 1].indices.push_back(index[i]);
	}

	free(v);
	free(index);

	return CreateVertexAndIndexBuffer();
}

int DrawPolygon::Create3Dbox(const float& width, const float& height, const float& depth, const bool& isFill)
{
	vertices.push_back({});

	// 前
	vertices[vertices.size() - 1].vertices.push_back({ { -width / 2, +height / 2, -depth / 2 }, {}, { 0.0f, 0.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { +width / 2, +height / 2, -depth / 2 }, {}, { 1.0f, 0.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { -width / 2, -height / 2, -depth / 2 }, {}, { 0.0f, 1.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { +width / 2, +height / 2, -depth / 2 }, {}, { 1.0f, 0.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { -width / 2, -height / 2, -depth / 2 }, {}, { 0.0f, 1.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { +width / 2, -height / 2, -depth / 2 }, {}, { 1.0f, 1.0f } });
	// 後ろ
	vertices[vertices.size() - 1].vertices.push_back({ { -width / 2, +height / 2, +depth / 2 }, {}, { 0.0f, 0.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { -width / 2, -height / 2, +depth / 2 }, {}, { 0.0f, 1.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { +width / 2, +height / 2, +depth / 2 }, {}, { 1.0f, 0.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { -width / 2, -height / 2, +depth / 2 }, {}, { 0.0f, 1.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { +width / 2, +height / 2, +depth / 2 }, {}, { 1.0f, 0.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { +width / 2, -height / 2, +depth / 2 }, {}, { 1.0f, 1.0f } });
	// 左
	vertices[vertices.size() - 1].vertices.push_back({ { -width / 2, -height / 2, +depth / 2 }, {}, { 0.0f, 0.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { -width / 2, +height / 2, +depth / 2 }, {}, { 1.0f, 0.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { -width / 2, -height / 2, -depth / 2 }, {}, { 0.0f, 1.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { -width / 2, +height / 2, +depth / 2 }, {}, { 1.0f, 0.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { -width / 2, -height / 2, -depth / 2 }, {}, { 0.0f, 1.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { -width / 2, +height / 2, -depth / 2 }, {}, { 1.0f, 1.0f } });
	// 右
	vertices[vertices.size() - 1].vertices.push_back({ { +width / 2, -height / 2, +depth / 2 }, {}, { 0.0f, 0.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { +width / 2, -height / 2, -depth / 2 }, {}, { 0.0f, 1.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { +width / 2, +height / 2, +depth / 2 }, {}, { 1.0f, 0.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { +width / 2, -height / 2, -depth / 2 }, {}, { 0.0f, 1.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { +width / 2, +height / 2, +depth / 2 }, {}, { 1.0f, 0.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { +width / 2, +height / 2, -depth / 2 }, {}, { 1.0f, 1.0f } });
	// 上
	vertices[vertices.size() - 1].vertices.push_back({ { +width / 2, -height / 2, -depth / 2 }, {}, { 0.0f, 0.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { +width / 2, -height / 2, +depth / 2 }, {}, { 1.0f, 0.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { -width / 2, -height / 2, -depth / 2 }, {}, { 0.0f, 1.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { +width / 2, -height / 2, +depth / 2 }, {}, { 1.0f, 0.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { -width / 2, -height / 2, -depth / 2 }, {}, { 0.0f, 1.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { -width / 2, -height / 2, +depth / 2 }, {}, { 1.0f, 1.0f } });
	// 下
	vertices[vertices.size() - 1].vertices.push_back({ { +width / 2, +height / 2, -depth / 2 }, {}, { 0.0f, 0.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { -width / 2, +height / 2, -depth / 2 }, {}, { 0.0f, 1.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { +width / 2, +height / 2, +depth / 2 }, {}, { 1.0f, 0.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { -width / 2, +height / 2, -depth / 2 }, {}, { 0.0f, 1.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { +width / 2, +height / 2, +depth / 2 }, {}, { 1.0f, 0.0f } });
	vertices[vertices.size() - 1].vertices.push_back({ { -width / 2, +height / 2, +depth / 2 }, {}, { 1.0f, 1.0f } });

	if (isFill == true)
	{
		for (unsigned short i = 0; i < 6; i++)
		{
			vertices[vertices.size() - 1].indices.push_back(0 + 3 * 2 * i);
			vertices[vertices.size() - 1].indices.push_back(1 + 3 * 2 * i);
			vertices[vertices.size() - 1].indices.push_back(2 + 3 * 2 * i);
			vertices[vertices.size() - 1].indices.push_back(4 + 3 * 2 * i);
			vertices[vertices.size() - 1].indices.push_back(3 + 3 * 2 * i);
			vertices[vertices.size() - 1].indices.push_back(5 + 3 * 2 * i);
		}
	}
	else
	{
		for (unsigned short i = 0; i < 6; i++)
		{
			vertices[vertices.size() - 1].indices.push_back(0 + 2 * 4 * i);
			vertices[vertices.size() - 1].indices.push_back(1 + 2 * 4 * i);
			vertices[vertices.size() - 1].indices.push_back(3 + 2 * 4 * i);
			vertices[vertices.size() - 1].indices.push_back(5 + 2 * 4 * i);
			vertices[vertices.size() - 1].indices.push_back(5 + 2 * 4 * i);
			vertices[vertices.size() - 1].indices.push_back(2 + 2 * 4 * i);
			vertices[vertices.size() - 1].indices.push_back(4 + 2 * 4 * i);
			vertices[vertices.size() - 1].indices.push_back(0 + 2 * 4 * i);
		}
	}

	return CreateVertexAndIndexBuffer();
}

int DrawPolygon::CreateCorn(const float& r, const float& h, const size_t& divNum, const bool& isFill)
{
	vertices.push_back({});

	Vertex* v = (Vertex*)malloc(sizeof(Vertex) * (divNum + 1));
	size_t indexSize = divNum * (2 + (size_t)isFill);
	unsigned short* index = (unsigned short*)malloc(sizeof(unsigned short) * indexSize);

	Circle({ 0, 0, 0 }, r, divNum, isFill, v, index);

	for (size_t i = 0; i < divNum + 1; i++)
	{
		vertices[vertices.size() - 1].vertices.push_back(v[i]);
	}
	for (size_t i = 0; i < indexSize; i++)
	{
		vertices[vertices.size() - 1].indices.push_back(index[i]);
	}

	free(v);
	free(index);

	size_t size = vertices[vertices.size() - 1].vertices.size();

	for (size_t i = 0; i < divNum; i++)
	{
		vertices[vertices.size() - 1].vertices.push_back(vertices[vertices.size() - 1].vertices[i + 0]);
		vertices[vertices.size() - 1].vertices.push_back(vertices[vertices.size() - 1].vertices[i + 1]);
		vertices[vertices.size() - 1].vertices.push_back({
			{
				vertices[vertices.size() - 1].vertices[divNum].pos.x,
				vertices[vertices.size() - 1].vertices[divNum].pos.y,
				vertices[vertices.size() - 1].vertices[divNum].pos.z + h
			},
			vertices[vertices.size() - 1].vertices[divNum].normal,
			vertices[vertices.size() - 1].vertices[divNum].uv,
			});

		if (isFill == true)
		{
			vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 3 + 0 + size));
			vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 3 + 1 + size));
			vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 3 + 2 + size));
		}
		else
		{
			vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 3 + 0 + size));
			vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 3 + 1 + size));
			vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 3 + 1 + size));
			vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 3 + 2 + size));
			vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 3 + 2 + size));
			vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 3 + 0 + size));
		}
	}

	vertices[vertices.size() - 1].vertices[divNum * 3 - 2 + size] =
		vertices[vertices.size() - 1].vertices[0];

	return CreateVertexAndIndexBuffer();
}

int DrawPolygon::CreateCylinder(const float& r, const float& h, const size_t& divNum, const bool& isFill)
{
	vertices.push_back({});

	Vertex* v = (Vertex*)malloc(sizeof(Vertex) * (divNum + 1));
	size_t indexSize = divNum * (2 + (size_t)isFill);
	unsigned short* index = (unsigned short*)malloc(sizeof(unsigned short) * indexSize);

	Circle({ 0, 0, 0 }, r, divNum, isFill, v, index);

	for (size_t i = 0; i < divNum + 1; i++)
	{
		vertices[vertices.size() - 1].vertices.push_back(v[i]);
	}
	for (size_t i = 0; i < indexSize; i++)
	{
		vertices[vertices.size() - 1].indices.push_back(index[i]);
	}

	size_t size1 = vertices[vertices.size() - 1].vertices.size();

	Circle({ 0, 0, h }, r, divNum, isFill, v, index);

	for (size_t i = 0; i < divNum + 1; i++)
	{
		vertices[vertices.size() - 1].vertices.push_back(v[i]);
	}
	for (size_t i = 0; i < indexSize; i++)
	{
		vertices[vertices.size() - 1].indices.push_back(index[i]);
	}

	free(v);
	free(index);

	size_t size2 = vertices[vertices.size() - 1].vertices.size();

	for (size_t i = 0; i < divNum; i++)
	{
		vertices[vertices.size() - 1].vertices.push_back(vertices[vertices.size() - 1].vertices[i + 0]);
		vertices[vertices.size() - 1].vertices.push_back(vertices[vertices.size() - 1].vertices[i + 1]);
		vertices[vertices.size() - 1].vertices.push_back(vertices[vertices.size() - 1].vertices[i + 0 + size1]);
		vertices[vertices.size() - 1].vertices.push_back(vertices[vertices.size() - 1].vertices[i + 1 + size1]);

		if (isFill == true)
		{
			vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 4 + 1 + size2));
			vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 4 + 0 + size2));
			vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 4 + 3 + size2));

			vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 4 + 3 + size2));
			vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 4 + 0 + size2));
			vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 4 + 2 + size2));
		}
		else
		{
			vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 4 + 0 + size2));
			vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 4 + 1 + size2));
			vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 4 + 2 + size2));
			vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 4 + 3 + size2));
		}
	}

	vertices[vertices.size() - 1].vertices[divNum * 4 - 3 + size2] = vertices[vertices.size() - 1].vertices[0];
	vertices[vertices.size() - 1].vertices[divNum * 4 - 1 + size2] = vertices[vertices.size() - 1].vertices[0 + size1];

	for (size_t i = 0; i < size1 - 1; i++)
	{
		vertices[vertices.size() - 1].vertices[i].pos.x *= -1;
		vertices[vertices.size() - 1].vertices[i].pos.z *= -1;
	}

	return CreateVertexAndIndexBuffer();
}

int DrawPolygon::CreateSphere(const float& r, const size_t& divNum, const bool& isFill)
{
	using namespace DirectX;

	vertices.push_back({});

	size_t sizeV = vertices[vertices.size() - 1].vertices.size();
	size_t oldsizeV = 0;
	size_t sizeI = vertices[vertices.size() - 1].indices.size();
	size_t oldsizeI = 0;
	size_t indexSize = divNum * 3;
	size_t divNum2 = divNum - 1;
	Vertex* v = (Vertex*)malloc(sizeof(Vertex) * (divNum + 1));
	unsigned short* index = (unsigned short*)malloc(sizeof(unsigned short) * indexSize);

	for (size_t i = 1; i < divNum2; i++)
	{
		float r2 = r * sinf(XM_PI / divNum2 * i);
		float h = r * cosf(XM_PI / divNum2 * i);

		if (r2 >= r * -0.00000005f && r2 <= r * 0.00000005f)
		{
			r2 = 0;
		}
		if (h >= r * -0.00000005f && h <= r * 0.00000005f)
		{
			h = 0;
		}

		Circle({ 0, 0, h }, r2, divNum, true, v, index);

		oldsizeV = sizeV;
		oldsizeI = sizeI;
		sizeV = vertices[vertices.size() - 1].vertices.size();
		sizeI = vertices[vertices.size() - 1].indices.size();

		if (i == 1)
		{
			v[divNum].pos.z = r;

			if (isFill == true)
			{
				for (size_t i = 0; i < divNum; i++)
				{
					vertices[vertices.size() - 1].vertices.push_back(v[i + 0]);
					vertices[vertices.size() - 1].vertices.push_back(v[i + 1]);
					vertices[vertices.size() - 1].vertices.push_back(v[divNum]);

					vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 3 + 1 + sizeV));
					vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 3 + 0 + sizeV));
					vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 3 + 2 + sizeV));
				}
				vertices[vertices.size() - 1].vertices[(divNum * 3 - 2) + sizeV] = v[0];
			}
			else
			{
				for (size_t j = 0; j < divNum + 1; j++)
				{
					vertices[vertices.size() - 1].vertices.push_back(v[j]);
				}

				for (size_t j = 0; j < divNum; j++)
				{
					vertices[vertices.size() - 1].indices.push_back((unsigned short)((j + 0) + sizeV));
					vertices[vertices.size() - 1].indices.push_back((unsigned short)((j + 1) + sizeV));
					vertices[vertices.size() - 1].indices.push_back((unsigned short)((j + 1) + sizeV));
					vertices[vertices.size() - 1].indices.push_back((unsigned short)(divNum + sizeV));
					vertices[vertices.size() - 1].indices.push_back((unsigned short)(divNum + sizeV));
					vertices[vertices.size() - 1].indices.push_back((unsigned short)((j + 0) + sizeV));
				}
				vertices[vertices.size() - 1].indices[(divNum * 6 - 4) + sizeV] = (unsigned short)sizeV;
				vertices[vertices.size() - 1].indices[(divNum * 6 - 5) + sizeV] = (unsigned short)sizeV;
			}
		}
		else
		{
			if (isFill == true)
			{
				for (size_t j = 0; j < divNum; j++)
				{
					if (i == 2)
					{
						vertices[vertices.size() - 1].vertices.push_back(v[j + 0]);
						vertices[vertices.size() - 1].vertices.push_back(v[j + 1]);
						vertices[vertices.size() - 1].vertices.push_back(vertices[vertices.size() - 1].vertices[j * 3 + 0 + oldsizeV]);
						vertices[vertices.size() - 1].vertices.push_back(vertices[vertices.size() - 1].vertices[j * 3 + 1 + oldsizeV]);
					}
					else
					{
						vertices[vertices.size() - 1].vertices.push_back(v[j + 0]);
						vertices[vertices.size() - 1].vertices.push_back(v[j + 1]);
						vertices[vertices.size() - 1].vertices.push_back(vertices[vertices.size() - 1].vertices[j * 4 + 0 + oldsizeV]);
						vertices[vertices.size() - 1].vertices.push_back(vertices[vertices.size() - 1].vertices[j * 4 + 1 + oldsizeV]);
					}

					vertices[vertices.size() - 1].indices.push_back((unsigned short)((j * 4 + 1) + sizeV));
					vertices[vertices.size() - 1].indices.push_back((unsigned short)((j * 4 + 0) + sizeV));
					vertices[vertices.size() - 1].indices.push_back((unsigned short)((j * 4 + 3) + sizeV));

					vertices[vertices.size() - 1].indices.push_back((unsigned short)((j * 4 + 3) + sizeV));
					vertices[vertices.size() - 1].indices.push_back((unsigned short)((j * 4 + 0) + sizeV));
					vertices[vertices.size() - 1].indices.push_back((unsigned short)((j * 4 + 2) + sizeV));
				}
				vertices[vertices.size() - 1].vertices[divNum * 4 + sizeV - 1] = vertices[vertices.size() - 1].vertices[oldsizeV];
				vertices[vertices.size() - 1].vertices[divNum * 4 + sizeV - 3] = v[0];
			}
			else
			{
				for (size_t j = 0; j < divNum; j++)
				{
					vertices[vertices.size() - 1].vertices.push_back(v[j]);

					vertices[vertices.size() - 1].indices.push_back((unsigned short)((j + 0) + oldsizeV));
					vertices[vertices.size() - 1].indices.push_back((unsigned short)((j + 0) + sizeV));
					vertices[vertices.size() - 1].indices.push_back((unsigned short)((j + 0) + sizeV));
					vertices[vertices.size() - 1].indices.push_back((unsigned short)((j + 1) + sizeV));
				}
				vertices[vertices.size() - 1].indices[(divNum * 4 - 1) + sizeI] = (unsigned short)sizeV;
			}
		}
	}
	oldsizeV = sizeV;
	oldsizeI = sizeI;
	sizeV = vertices[vertices.size() - 1].vertices.size();
	sizeI = vertices[vertices.size() - 1].indices.size();

	v[divNum].pos.z = -r;

	if (isFill == true)
	{
		for (size_t i = 0; i < divNum; i++)
		{
			vertices[vertices.size() - 1].vertices.push_back(v[i + 0]);
			vertices[vertices.size() - 1].vertices.push_back(v[i + 1]);
			vertices[vertices.size() - 1].vertices.push_back(v[divNum]);

			vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 3 + 0 + sizeV));
			vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 3 + 1 + sizeV));
			vertices[vertices.size() - 1].indices.push_back((unsigned short)(i * 3 + 2 + sizeV));
		}
		vertices[vertices.size() - 1].vertices[(divNum * 3 - 2) + sizeV] = v[0];
	}
	else
	{
		for (size_t i = 0; i < divNum; i++)
		{
			if (divNum == 3)
			{
				vertices[vertices.size() - 1].indices.push_back((unsigned short)((i + 0) + oldsizeV));
				vertices[vertices.size() - 1].indices.push_back((unsigned short)(divNum + 1 + oldsizeV));
			}
			else
			{
				vertices[vertices.size() - 1].indices.push_back((unsigned short)((i + 0) + oldsizeV));
				vertices[vertices.size() - 1].indices.push_back((unsigned short)(divNum + oldsizeV));
			}
		}
		vertices[vertices.size() - 1].vertices.push_back(v[divNum]);
	}

	free(v);
	free(index);

	return CreateVertexAndIndexBuffer();
}

void DrawPolygon::NormalizeUV(const int& polygonData, const int& graphHandle)
{
	if ((polygonData < 0 || (size_t)polygonData >= vertices.size()) ||
		(graphHandle < 0 || (size_t)graphHandle >= textrueData.size()))
	{
		return;
	}

	// テクスチャデータ取得
	D3D12_RESOURCE_DESC resDesc = textrueData[graphHandle].texbuff->GetDesc();
	float left = 0, right = 0, top = 0, bottom = 0;

	for (size_t i = 0; i < vertices[polygonData].vertices.size(); i++)
	{
		if (vertices[polygonData].vertices[i].pos.x < left)
		{
			left = vertices[polygonData].vertices[i].pos.x;
		}
		if (vertices[polygonData].vertices[i].pos.x > right)
		{
			right = vertices[polygonData].vertices[i].pos.x;
		}
		if (vertices[polygonData].vertices[i].pos.y < top)
		{
			top = vertices[polygonData].vertices[i].pos.y;
		}
		if (vertices[polygonData].vertices[i].pos.y > bottom)
		{
			bottom = vertices[polygonData].vertices[i].pos.y;
		}
	}
	DirectX::XMFLOAT2 size = { right - left, bottom - top };
	size = { size.x / resDesc.Width, size.y / resDesc.Height };

	for (size_t i = 0; i < vertices[polygonData].vertices.size(); i++)
	{
		vertices[polygonData].vertices[i].uv.x *= size.x;
		vertices[polygonData].vertices[i].uv.y *= size.y;
	}

	vertices[polygonData].vertBuff->Map(0, nullptr, (void**)&vertMap);

	// 全頂点に対して
	for (UINT i = 0; i < vertices[polygonData].vertices.size(); i++)
	{
		vertMap[i] = vertices[polygonData].vertices[i]; //座標をコピー
	}

	// マップを解除
	vertices[polygonData].vertBuff->Unmap(0, nullptr);
}

void DrawPolygon::MoveUV(const int& polygonData, const int& graphHandle, DirectX::XMFLOAT2 speed)
{
	if ((polygonData < 0 || (size_t)polygonData >= vertices.size()) ||
		(graphHandle < 0 || (size_t)graphHandle >= textrueData.size()))
	{
		return;
	}

	// テクスチャデータ取得
	D3D12_RESOURCE_DESC resDesc = textrueData[graphHandle].texbuff->GetDesc();
	DirectX::XMFLOAT2 addUV = { speed.x / resDesc.Width, speed.y / resDesc.Height };

	for (size_t i = 0; i < vertices[polygonData].vertices.size(); i++)
	{
		vertices[polygonData].vertices[i].uv.x += addUV.x;
		vertices[polygonData].vertices[i].uv.y += addUV.y;
	}

	vertices[polygonData].vertBuff->Map(0, nullptr, (void**)&vertMap);

	// 全頂点に対して
	for (UINT i = 0; i < vertices[polygonData].vertices.size(); i++)
	{
		vertMap[i] = vertices[polygonData].vertices[i]; //座標をコピー
	}

	// マップを解除
	vertices[polygonData].vertBuff->Unmap(0, nullptr);
}

int DrawPolygon::DrawPolygonInit()
{
	HRESULT hr = S_FALSE;
	static bool isDrawPolygonInit = false;

	int index = (int)obj.size();
	hr = CreateConstBuffer(&index);
	if (FAILED(hr))
	{
		return EoF;
	}

	if (isDrawPolygonInit == false)
	{
		isDrawPolygonInit = true;

		if (LoadTextrue() == EoF)
		{
			return EoF;
		}

		hr = Init();
		if (FAILED(hr))
		{
			return EoF;
		}
	}

	return index;
}

int DrawPolygon::Draw(
	const int& polygonData, const XMFLOAT3& position, const XMMATRIX& rotation, const XMFLOAT3& scale,
	const XMFLOAT4& color, const int& graphHandle, const bool& isFill,
	const bool& isOrthoGraphic, const bool& viewMultiFlag, const int& parent)
{
	if ((polygonData < 0 || (size_t)polygonData >= vertices.size()) ||
		(graphHandle < 0 || (UINT)graphHandle > texCommonData.textrueCount) ||
		(parent < -1 && (parent != -1 && (size_t)parent >= obj.size())))
	{
		return EoF;
	}

	using namespace DirectX;

	bool isInit = false;
	XMMATRIX mat = XMMatrixIdentity();

	if ((size_t)(polygonCount + 1) < objIndex.size())
	{
		isInit = true;
	}

	if (isInit == false)
	{
		int size = DrawPolygonInit();
		if (size == EoF)
		{
			return EoF;
		}

		objIndex.push_back({ size, graphHandle });

		if (isFill == false)
		{
			for (size_t i = 0; i < vertices[polygonData].vertices.size(); i++)
			{
				vertices[polygonData].vertices[i].normal = { -light.x, -light.y, -light.z };
			}
		}
	}

	if (objIndex.size() <= 0)
	{
		return EoF;
	}

	polygonCount++;
	polygonCount %= objIndex.size();

	IndexData& index = objIndex[polygonCount];

	UpdataConstant(position, rotation, scale, index.constant, polygonData, parent);

	if (index.textrue != graphHandle)
	{
		index.textrue = graphHandle;
	}

	if (viewMultiFlag == true)
	{
		mat = matView[cameraNo];
	}
	else
	{
		mat = XMMatrixIdentity();
	}

	if (isOrthoGraphic == true)
	{
		mat *= objectData.matProjection[CommonData::Projection::ORTHOGRAPHIC];
		lightVec = {
			-vertices[polygonData].vertices[0].normal.x,
			-vertices[polygonData].vertices[0].normal.y,
			-vertices[polygonData].vertices[0].normal.z
		};
	}
	else
	{
		mat *= objectData.matProjection[CommonData::Projection::PERSPECTIVE];
		lightVec = light;
	}

#pragma region GraphicsCommand

	BaseDrawGraphics();

	ConstBufferData* constMap = nullptr;
	obj[index.constant].constBuff->Map(0, nullptr, (void**)&constMap); //マッピング

	constMap->color = color;
	constMap->mat = obj[index.constant].matWorld * mat;
	constMap->lightVec = lightVec;
	obj[index.constant].constBuff->Unmap(0, nullptr); //マッピング解除

	// デスクリプタヒープをセット
	ID3D12DescriptorHeap* ppHeaps[] = { texCommonData.descHeap.Get() };
	w->cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	w->cmdList->SetGraphicsRootConstantBufferView(0, obj[index.constant].constBuff->GetGPUVirtualAddress());
	w->cmdList->SetGraphicsRootDescriptorTable(1, textrueData[index.textrue].gpuDescHandle);

	if (isFill == true)
	{
		// プリミティブ形状の設定
		w->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	else
	{
		// プリミティブ形状の設定
		w->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	}

	// 頂点バッファの設定
	w->cmdList->IASetVertexBuffers(0, 1, &vertices[polygonData].vbView);
	// インデックスバッファビューの設定
	w->cmdList->IASetIndexBuffer(&vertices[polygonData].ibView);
	// 描画コマンド
	w->cmdList->DrawIndexedInstanced((UINT)vertices[polygonData].indices.size(), 1, 0, 0, 0);

	return index.constant;
}

int DrawPolygon::CreateCamera(const XMFLOAT3& cameraPos, const XMFLOAT3& cameraTarget, const XMFLOAT3& upVector)
{
	using namespace DirectX;

	matView.push_back(Camera::CreateCamera(
		XMLoadFloat3(&cameraPos),
		XMLoadFloat3(&cameraTarget),
		XMLoadFloat3(&upVector)
	));

	return (int)matView.size() - 1;
}

void DrawPolygon::SetCamera(const XMFLOAT3& cameraPos, const XMFLOAT3& cameraTarget, const XMFLOAT3& upVector,
	const int& cameraNo)
{
	if (cameraNo < 0 || (size_t)cameraNo >= matView.size())
	{
		return;
	}

	using namespace DirectX;

	matView[cameraNo] = Camera::CreateCamera(
		XMLoadFloat3(&cameraPos),
		XMLoadFloat3(&cameraTarget),
		XMLoadFloat3(&upVector)
	);
}

void DrawPolygon::ChangeCamera(const int& cameraNo)
{
	if (cameraNo < 0 || (size_t)cameraNo >= matView.size())
	{
		return;
	}

	this->cameraNo = cameraNo;
}

DirectX::XMFLOAT3 DrawPolygon::ScreenToWorld(int x, int y, float z)
{
	using namespace DirectX;

	XMFLOAT3 world = {};
	XMFLOAT3 screen = { (float)x, (float)y, z };
	XMMATRIX InvView, InvPrj, VP, InvViewport;

	InvView = XMMatrixInverse(nullptr, matView[cameraNo]);
	InvPrj = XMMatrixInverse(nullptr, objectData.matProjection[CommonData::Projection::PERSPECTIVE]);
	VP = XMMatrixIdentity();
	VP.r[0].m128_f32[0] = w->GetWindowWidthF() / 2.0f;
	VP.r[1].m128_f32[1] = -(w->GetWindowHeightF()) / 2.0f;
	VP.r[3].m128_f32[0] = w->GetWindowWidthF() / 2.0f;
	VP.r[3].m128_f32[1] = w->GetWindowHeightF() / 2.0f;
	InvViewport = XMMatrixInverse(nullptr, VP);

	XMMATRIX temp = InvViewport * InvPrj * InvView;
	XMStoreFloat3(&world, XMVector3TransformCoord(XMLoadFloat3(&screen), temp));

	return world;
}

DirectX::XMMATRIX DrawPolygon::InverseView()
{
	DirectX::XMMATRIX invView = DirectX::XMMatrixInverse(nullptr, matView[cameraNo]);
	invView.r[3].m128_f32[0] = 0.0f;
	invView.r[3].m128_f32[1] = 0.0f;
	invView.r[3].m128_f32[2] = 0.0f;
	return invView;
}

void DrawPolygon::PolygonLoopEnd()
{
	loopCount++;

	TextLoopEnd();

	polygonCount = -1;
}

void DrawPolygon::Circle(const XMFLOAT3& centerPos, const float& r, const size_t& divNum,
	const bool& flag, Vertex* v, unsigned short* index)
{
	using namespace DirectX;

	if (v == nullptr || index == nullptr)
	{
		return;
	}

	for (size_t i = 0; i < divNum; i++)
	{
		/*頂点座標*/
		v[i].pos.x = r * sinf(2 * XM_PI / divNum * i) + centerPos.x;
		v[i].pos.y = r * cosf(2 * XM_PI / divNum * i) + centerPos.y;
		v[i].pos.z = centerPos.z;
		v[i].normal = {};
		v[i].uv.x = (sinf(2 * XM_PI / divNum * i) + 1.0f) / 2.0f;
		v[i].uv.y = (cosf(2 * XM_PI / divNum * i) + 1.0f) / 2.0f;

		if (v[i].pos.x >= r * -0.00000005f && v[i].pos.x <= r * 0.00000005f)
		{
			v[i].pos.x = 0;
		}
		if (v[i].pos.y >= r * -0.00000005f && v[i].pos.y <= r * 0.00000005f)
		{
			v[i].pos.y = 0;
		}
		if (v[i].uv.x >= -0.00000005f && v[i].uv.x <= 0.00000005f)
		{
			v[i].uv.x = 0;
		}
		if (v[i].uv.y >= -0.00000005f && v[i].uv.y <= 0.00000005f)
		{
			v[i].uv.y = 0;
		}

		if (flag == true)
		{
			/*fill用インデックスデータ*/
			index[i * 3 + 0] = (unsigned short)(i + 1);
			index[i * 3 + 1] = (unsigned short)(i + 0);
			index[i * 3 + 2] = (unsigned short)(divNum);
		}
		else
		{
			/*line用インデックスデータ*/
			index[i * 2 + 0] = (unsigned short)(i + 1);
			index[i * 2 + 1] = (unsigned short)(i + 0);
		}
	}

	// verticesのdivNum番目にcenterPosを代入
	v[divNum].pos = { centerPos.x, centerPos.y, centerPos.z };
	v[divNum].normal = {};
	v[divNum].uv = { 0.5f, 0.5f };

	for (size_t i = 0; i < divNum + 1; i++)
	{
		v[i].uv.y *= -1;
	}

	/*インデックスデータ調整*/
	if (flag == true)
	{
		/*fill用インデックスデータ*/
		index[(divNum - 1) * 3] = (unsigned short)0; //一番最後の手前の要素を'0'に置き換え
	}
	else
	{
		/*line用インデックスデータ*/
		index[(divNum - 1) * 2] = (unsigned short)0; //一番最後の要素を'0'に置き換え
	}

	return;
}
