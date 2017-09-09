#include <sfl/Object.h>

namespace sfl
{
	Line Object::GetLine(int i)
	{
		Line ret;
		ret.Create(Points[i], Points[(i == Points.size() - 1) ? 0 : (i + 1)]);
		return ret;
	}
}