#include <nlohmann/json.hpp>

#include <Eigen/Core>
#include <Eigen/Geometry>

///////////////////////////////////// EIGEN DEFINITIONS /////////////////////////////////////
namespace nlohmann
{
	template<typename FloatingPoint, unsigned Elements>
	struct adl_serializer<Eigen::Vector<FloatingPoint, Elements>>
	{
		using Vector = Eigen::Vector<FloatingPoint, Elements>;

		static Vector from_json(nlohmann::json const& json)
		{
			Vector output;
			char buffer[] = "0";
			for (unsigned i = 0; i < Elements; ++i)
			{
				buffer[0] = static_cast<char>('0' + i);
				output[i] = json[buffer];
			}
			return output;
		}

		static void to_json(nlohmann::json& json, Vector const& v)
		{
			char buffer[] = "0";
			for (unsigned i = 0; i < Elements; ++i)
			{
				buffer[0] = static_cast<char>('0' + i);
				json[buffer] = v[i];
			}
		}
	};
}

namespace nlohmann
{
	template<typename FloatingPoint, auto Other>
	struct adl_serializer<Eigen::Quaternion<FloatingPoint, Other>>
	{
		using Quaternion = Eigen::Quaternion<FloatingPoint, Other>;

		static Quaternion from_json(nlohmann::json const& json)
		{
			Quaternion quaternion;
			char buffer[] = "0";
			for (unsigned i = 0; i < 4; ++i)
			{
				buffer[0] = static_cast<char>('0' + i);
				quaternion.coeffs()[i] = json[buffer];
			}
			return quaternion;
		}

		static void to_json(nlohmann::json& json, Quaternion const& q)
		{
			char buffer[] = "0";
			for (unsigned i = 0; i < 4; ++i)
			{
				buffer[0] = static_cast<char>('0' + i);
				json[buffer] = q.coeffs()[i];
			}
		}
	};
}
///////////////////////////////////// EIGEN DEFINITIONS /////////////////////////////////////

///////////////////////////////////// STRUCTURE INTERFACE /////////////////////////////////////
using Vector3 = Eigen::Vector3f;
using Quaternion = Eigen::Quaternionf;

struct Thing
{
private:
	Vector3 v_;
	Quaternion q_;

public:
	Thing(Vector3 const& v, Quaternion const& q) : v_(v), q_(q) {}
	Vector3 const& GetV() const noexcept { return v_; }
	Quaternion const& GetQ() const noexcept { return q_; }
};
///////////////////////////////////// STRUCTURE INTERFACE /////////////////////////////////////

///////////////////////////////////// STRUCTURE SERIALIZATION INTERFACE /////////////////////////////////////
namespace nlohmann
{
	template<>
	struct adl_serializer<Thing>
	{
		static Thing from_json(nlohmann::json const& json);
		static void to_json(nlohmann::json& json, Thing const& ray);
	};
}
///////////////////////////////////// STRUCTURE SERIALIZATION INTERFACE /////////////////////////////////////

///////////////////////////////////// STRUCTURE SERIALIZATION IMPLEMENTATION /////////////////////////////////////
namespace nlohmann
{
	Thing adl_serializer<Thing>::from_json(nlohmann::json const& json)
	{
		return Thing
		{
			json["V"].get<Vector3>(),
			json["Q"].get<Quaternion>(),
		};
	}

	void adl_serializer<Thing>::to_json(nlohmann::json& json, Thing const& thing)
	{
		Vector3 v = thing.GetV();
		nlohmann::adl_serializer<Vector3>::to_json(json["V"], v);
		nlohmann::to_json(json["V"], thing.GetV());
		json["V"] = v;

		Quaternion q = thing.GetQ();
		nlohmann::adl_serializer<Quaternion>::to_json(json["Q"], q);
		nlohmann::to_json(json["Q"], q);
		json["Q"] = q;
	}
}
///////////////////////////////////// STRUCTURE SERIALIZATION IMPLEMENTATION /////////////////////////////////////

///////////////////////////////////// STRUCTURE ALSO DOESN'T SERIALIZE /////////////////////////////////////
namespace 
{
	void UseThingToJson(nlohmann::json & json, Thing & thing)
	{
		nlohmann::adl_serializer<Thing>::to_json(json["Thing"], thing);
		nlohmann::to_json(json["Thing"], thing);
		json["Thing"] = thing;
	}
}
///////////////////////////////////// STRUCTURE ALSO DOESN'T SERIALIZE /////////////////////////////////////

int main()
{
	
}