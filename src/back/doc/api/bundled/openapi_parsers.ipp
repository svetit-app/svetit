#pragma once

#include "openapi.hpp"

    #include <userver/chaotic/array.hpp>
    #include <userver/chaotic/exception.hpp>
    #include <userver/chaotic/object.hpp>
    #include <userver/chaotic/primitive.hpp>
    #include <userver/chaotic/validators.hpp>
    #include <userver/chaotic/with_type.hpp>
    #include <userver/formats/parse/common_containers.hpp>
    #include <userver/formats/serialize/common_containers.hpp>
    #include <userver/utils/trivial_map.hpp>










    
    
        
        namespace svetit {namespace api {
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__CcDi_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("ccId").Case("diId");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::CcDi Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::CcDi>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::CcDi res;

            res.ccId =
                    value["ccId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcDi::kCcidMinimum>>>
                    ();res.diId =
                    value["diId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcDi::kDiidMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__CcDi_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__CcDis_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::CcDis Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::CcDis>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::CcDis res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::CcDi>, std::vector<svetit::api::CcDi>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcDis::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__CcDis_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__CcModeType_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("id").Case("ccTypeId").Case("key").Case("name");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::CcModeType Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::CcModeType>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::CcModeType res;

            res.id =
                    value["id"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcModeType::kIdMinimum>>>>
                    ();res.ccTypeId =
                    value["ccTypeId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcModeType::kCctypeidMinimum>>>
                    ();res.key =
                    value["key"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>>
                    ();res.name =
                    value["name"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__CcModeType_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__CcModeTypes_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::CcModeTypes Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::CcModeTypes>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::CcModeTypes res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::CcModeType>, std::vector<svetit::api::CcModeType>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcModeTypes::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__CcModeTypes_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__CcParam_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("ccId").Case("paramId");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::CcParam Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::CcParam>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::CcParam res;

            res.ccId =
                    value["ccId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcParam::kCcidMinimum>>>
                    ();res.paramId =
                    value["paramId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcParam::kParamidMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__CcParam_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__CcParams_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::CcParams Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::CcParams>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::CcParams res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::CcParam>, std::vector<svetit::api::CcParam>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcParams::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__CcParams_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__CcStatusCategory_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("id").Case("projectId").Case("key").Case("name").Case("color");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::CcStatusCategory Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::CcStatusCategory>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::CcStatusCategory res;

            res.id =
                    value["id"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcStatusCategory::kIdMinimum>>>>
                    ();res.projectId =
                    value["projectId"].template As<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>
                    ();res.key =
                    value["key"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>>
                    ();res.name =
                    value["name"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>>
                    ();res.color =
                    value["color"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<16>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__CcStatusCategory_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__CcStatusCategories_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::CcStatusCategories Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::CcStatusCategories>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::CcStatusCategories res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::CcStatusCategory>, std::vector<svetit::api::CcStatusCategory>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcStatusCategories::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__CcStatusCategories_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__CcStatusType_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("id").Case("ccTypeId").Case("categoryId").Case("key").Case("text").Case("inform");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::CcStatusType Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::CcStatusType>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::CcStatusType res;

            res.id =
                    value["id"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcStatusType::kIdMinimum>>>>
                    ();res.ccTypeId =
                    value["ccTypeId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcStatusType::kCctypeidMinimum>>>
                    ();res.categoryId =
                    value["categoryId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcStatusType::kCategoryidMinimum>>>
                    ();res.key =
                    value["key"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>>
                    ();res.text =
                    value["text"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<512>>>
                    ();res.inform =
                    value["inform"].template As<USERVER_NAMESPACE::chaotic::Primitive<bool>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__CcStatusType_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__CcStatusTypes_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::CcStatusTypes Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::CcStatusTypes>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::CcStatusTypes res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::CcStatusType>, std::vector<svetit::api::CcStatusType>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcStatusTypes::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__CcStatusTypes_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__CcType_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("id").Case("projectId").Case("key").Case("name").Case("description");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::CcType Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::CcType>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::CcType res;

            res.id =
                    value["id"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcType::kIdMinimum>>>>
                    ();res.projectId =
                    value["projectId"].template As<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>
                    ();res.key =
                    value["key"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>>
                    ();res.name =
                    value["name"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>>
                    ();res.description =
                    value["description"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__CcType_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__CcTypeDiType_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("ccTypeId").Case("diTypeId");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::CcTypeDiType Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::CcTypeDiType>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::CcTypeDiType res;

            res.ccTypeId =
                    value["ccTypeId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcTypeDiType::kCctypeidMinimum>>>
                    ();res.diTypeId =
                    value["diTypeId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcTypeDiType::kDitypeidMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__CcTypeDiType_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__CcTypeDiTypes_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::CcTypeDiTypes Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::CcTypeDiTypes>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::CcTypeDiTypes res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::CcTypeDiType>, std::vector<svetit::api::CcTypeDiType>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcTypeDiTypes::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__CcTypeDiTypes_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__CcTypeParam_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("ccTypeId").Case("paramId");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::CcTypeParam Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::CcTypeParam>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::CcTypeParam res;

            res.ccTypeId =
                    value["ccTypeId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcTypeParam::kCctypeidMinimum>>>
                    ();res.paramId =
                    value["paramId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcTypeParam::kParamidMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__CcTypeParam_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__CcTypeParams_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::CcTypeParams Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::CcTypeParams>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::CcTypeParams res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::CcTypeParam>, std::vector<svetit::api::CcTypeParam>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcTypeParams::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__CcTypeParams_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__CcTypes_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::CcTypes Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::CcTypes>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::CcTypes res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::CcType>, std::vector<svetit::api::CcType>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcTypes::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__CcTypes_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Code_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("id").Case("projectId").Case("repositoryId").Case("commitHash");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Code Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Code>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Code res;

            res.id =
                    value["id"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Code::kIdMinimum>>>>
                    ();res.projectId =
                    value["projectId"].template As<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>
                    ();res.repositoryId =
                    value["repositoryId"].template As<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>
                    ();res.commitHash =
                    value["commitHash"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Code_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Codes_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Codes Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Codes>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Codes res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Code>, std::vector<svetit::api::Code>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Codes::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Codes_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__ControlCircuit_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("id").Case("typeId").Case("sectionId").Case("name");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::ControlCircuit Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::ControlCircuit>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::ControlCircuit res;

            res.id =
                    value["id"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::ControlCircuit::kIdMinimum>>>>
                    ();res.typeId =
                    value["typeId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::ControlCircuit::kTypeidMinimum>>>
                    ();res.sectionId =
                    value["sectionId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::ControlCircuit::kSectionidMinimum>>>
                    ();res.name =
                    value["name"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__ControlCircuit_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__ControlCircuits_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::ControlCircuits Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::ControlCircuits>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::ControlCircuits res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::ControlCircuit>, std::vector<svetit::api::ControlCircuit>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::ControlCircuits::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__ControlCircuits_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Device_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("id").Case("projectId").Case("pluginId").Case("name").Case("checkIntervalMsec");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Device Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Device>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Device res;

            res.id =
                    value["id"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Device::kIdMinimum>>>>
                    ();res.projectId =
                    value["projectId"].template As<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>
                    ();res.pluginId =
                    value["pluginId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Device::kPluginidMinimum>>>
                    ();res.name =
                    value["name"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>>
                    ();res.checkIntervalMsec =
                    value["checkIntervalMsec"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int32_t>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Device_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__DeviceItem_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("id").Case("deviceId").Case("typeId").Case("name");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::DeviceItem Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::DeviceItem>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::DeviceItem res;

            res.id =
                    value["id"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DeviceItem::kIdMinimum>>>>
                    ();res.deviceId =
                    value["deviceId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DeviceItem::kDeviceidMinimum>>>
                    ();res.typeId =
                    value["typeId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DeviceItem::kTypeidMinimum>>>
                    ();res.name =
                    value["name"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__DeviceItem_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__DeviceItems_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::DeviceItems Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::DeviceItems>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::DeviceItems res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::DeviceItem>, std::vector<svetit::api::DeviceItem>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DeviceItems::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__DeviceItems_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__DevicePluginParam_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("deviceId").Case("paramId");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::DevicePluginParam Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::DevicePluginParam>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::DevicePluginParam res;

            res.deviceId =
                    value["deviceId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DevicePluginParam::kDeviceidMinimum>>>
                    ();res.paramId =
                    value["paramId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DevicePluginParam::kParamidMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__DevicePluginParam_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__DevicePluginParams_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::DevicePluginParams Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::DevicePluginParams>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::DevicePluginParams res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::DevicePluginParam>, std::vector<svetit::api::DevicePluginParam>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DevicePluginParams::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__DevicePluginParams_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Devices_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Devices Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Devices>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Devices res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Device>, std::vector<svetit::api::Device>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Devices::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Devices_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__DiPluginParam_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("diTypeId").Case("paramId");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::DiPluginParam Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::DiPluginParam>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::DiPluginParam res;

            res.diTypeId =
                    value["diTypeId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DiPluginParam::kDitypeidMinimum>>>
                    ();res.paramId =
                    value["paramId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DiPluginParam::kParamidMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__DiPluginParam_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__DiPluginParams_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::DiPluginParams Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::DiPluginParams>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::DiPluginParams res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::DiPluginParam>, std::vector<svetit::api::DiPluginParam>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DiPluginParams::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__DiPluginParams_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        static constexpr USERVER_NAMESPACE::utils::TrivialBiMap ksvetit__api__DiType__Mode_Mapping =
            [](auto selector) {
            return selector().template Type<svetit::api::DiType::Mode, std::string_view>()
                    .Case(svetit::api::DiType::Mode::kReadonlyflag, "readonlyFlag")
                    .Case(svetit::api::DiType::Mode::kReadwriteflag, "readwriteFlag")
                    .Case(svetit::api::DiType::Mode::kReadonly, "readonly")
                    .Case(svetit::api::DiType::Mode::kReadwrite, "readwrite")
                    .Case(svetit::api::DiType::Mode::kFile, "file")
                    .Case(svetit::api::DiType::Mode::kButton, "button")
                    .Case(svetit::api::DiType::Mode::kVideostream, "videoStream");
        };
    

    
    

    
        static constexpr USERVER_NAMESPACE::utils::TrivialBiMap ksvetit__api__DiType__Savealgorithm_Mapping =
            [](auto selector) {
            return selector().template Type<svetit::api::DiType::Savealgorithm, std::string_view>()
                    .Case(svetit::api::DiType::Savealgorithm::kOff, "off")
                    .Case(svetit::api::DiType::Savealgorithm::kImmediately, "immediately")
                    .Case(svetit::api::DiType::Savealgorithm::kBytimer, "byTimer")
                    .Case(svetit::api::DiType::Savealgorithm::kBytimerorimmediately, "byTimerOrImmediately");
        };
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__DiType_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("id").Case("measureId").Case("saveTimerId").Case("key").Case("name").Case("mode").Case("saveAlgorithm");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
	template<typename Value>
        svetit::api::DiType::Mode Parse(Value val,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType::Mode>)
        {
            const auto value = val.template As<std::string>();
            const auto result = ksvetit__api__DiType__Mode_Mapping.TryFindBySecond(value);
            if (result.has_value()) {
                return *result;
            }
            USERVER_NAMESPACE::chaotic::ThrowForValue(fmt::format("Invalid enum value ({}) for type svetit::api::DiType::Mode", value), val);
        }
    

    
    

    
	template<typename Value>
        svetit::api::DiType::Savealgorithm Parse(Value val,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType::Savealgorithm>)
        {
            const auto value = val.template As<std::string>();
            const auto result = ksvetit__api__DiType__Savealgorithm_Mapping.TryFindBySecond(value);
            if (result.has_value()) {
                return *result;
            }
            USERVER_NAMESPACE::chaotic::ThrowForValue(fmt::format("Invalid enum value ({}) for type svetit::api::DiType::Savealgorithm", value), val);
        }
    

    

    
        
	template<typename Value>
        svetit::api::DiType Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::DiType res;

            res.id =
                    value["id"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DiType::kIdMinimum>>>>
                    ();res.measureId =
                    value["measureId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DiType::kMeasureidMinimum>>>
                    ();res.saveTimerId =
                    value["saveTimerId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DiType::kSavetimeridMinimum>>>
                    ();res.key =
                    value["key"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>>
                    ();res.name =
                    value["name"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>>
                    ();res.mode =
                    value["mode"].template As<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::DiType::Mode>>
                    ();res.saveAlgorithm =
                    value["saveAlgorithm"].template As<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::DiType::Savealgorithm>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__DiType_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__DiTypes_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::DiTypes Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::DiTypes>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::DiTypes res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::DiType>, std::vector<svetit::api::DiType>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DiTypes::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__DiTypes_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Error_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("code").Case("message");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Error Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Error>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Error res;

            res.code =
                    value["code"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int32_t>>
                    ();res.message =
                    value["message"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Error_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Group_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("id").Case("name").Case("description").Case("spaceId");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Group Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Group>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Group res;

            res.id =
                    value["id"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::int32_t>>>
                    ();res.name =
                    value["name"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>>
                    ();res.description =
                    value["description"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string>>
                    ();res.spaceId =
                    value["spaceId"].template As<std::optional<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Group_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__GroupUser_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("nodeId").Case("userId");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::GroupUser Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::GroupUser>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::GroupUser res;

            res.nodeId =
                    value["nodeId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int32_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::GroupUser::kNodeidMinimum>>>
                    ();res.userId =
                    value["userId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<40>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__GroupUser_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__GroupUsers_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::GroupUsers Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::GroupUsers>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::GroupUsers res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::GroupUser>, std::vector<svetit::api::GroupUser>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::GroupUsers::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__GroupUsers_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Groups_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Groups Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Groups>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Groups res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Group>, std::vector<svetit::api::Group>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Groups::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Groups_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Invitation_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("id").Case("spaceId").Case("creatorId").Case("userId").Case("roleId").Case("createdAt");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Invitation Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Invitation>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Invitation res;

            res.id =
                    value["id"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::int32_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Invitation::kIdMinimum>>>>
                    ();res.spaceId =
                    value["spaceId"].template As<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>
                    ();res.creatorId =
                    value["creatorId"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>>
                    ();res.userId =
                    value["userId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>
                    ();res.roleId =
                    value["roleId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int32_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Invitation::kRoleidMinimum>>>
                    ();res.createdAt =
                    value["createdAt"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Invitation_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__InvitationRole_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("roleId");
                };

        
    


    
    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::InvitationRole Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::InvitationRole>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::InvitationRole res;

            res.roleId =
                    value["roleId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int32_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::InvitationRole::kRoleidMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__InvitationRole_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Invitations_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Invitations Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Invitations>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Invitations res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Invitation>, std::vector<svetit::api::Invitation>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Invitations::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Invitations_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Link_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("id").Case("spaceId").Case("creatorId").Case("name").Case("createdAt").Case("expiredAt");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Link Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Link>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Link res;

            res.id =
                    value["id"].template As<std::optional<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>>
                    ();res.spaceId =
                    value["spaceId"].template As<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>
                    ();res.creatorId =
                    value["creatorId"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>>
                    ();res.name =
                    value["name"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>
                    ();res.createdAt =
                    value["createdAt"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>>>
                    ();res.expiredAt =
                    value["expiredAt"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Link_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Links_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Links Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Links>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Links res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Link>, std::vector<svetit::api::Link>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Links::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Links_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Measure_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("id").Case("projectId").Case("name");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Measure Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Measure>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Measure res;

            res.id =
                    value["id"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Measure::kIdMinimum>>>>
                    ();res.projectId =
                    value["projectId"].template As<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>
                    ();res.name =
                    value["name"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<10>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Measure_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Measures_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Measures Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Measures>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Measures res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Measure>, std::vector<svetit::api::Measure>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Measures::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Measures_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Node_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("id").Case("spaceId").Case("name").Case("description").Case("latitude").Case("longitude").Case("createdAt");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Node Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Node>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Node res;

            res.id =
                    value["id"].template As<std::optional<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>>
                    ();res.spaceId =
                    value["spaceId"].template As<std::optional<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>>
                    ();res.name =
                    value["name"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>>
                    ();res.description =
                    value["description"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string>>
                    ();res.latitude =
                    value["latitude"].template As<USERVER_NAMESPACE::chaotic::Primitive<double>>
                    ();res.longitude =
                    value["longitude"].template As<USERVER_NAMESPACE::chaotic::Primitive<double>>
                    ();res.createdAt =
                    value["createdAt"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Node_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__NodeGroup_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("nodeId").Case("groupId");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::NodeGroup Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::NodeGroup>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::NodeGroup res;

            res.nodeId =
                    value["nodeId"].template As<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>
                    ();res.groupId =
                    value["groupId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int32_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::NodeGroup::kGroupidMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__NodeGroup_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__NodeGroups_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::NodeGroups Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::NodeGroups>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::NodeGroups res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::NodeGroup>, std::vector<svetit::api::NodeGroup>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::NodeGroups::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__NodeGroups_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__NodeProject_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("nodeId").Case("projectId");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::NodeProject Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::NodeProject>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::NodeProject res;

            res.nodeId =
                    value["nodeId"].template As<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>
                    ();res.projectId =
                    value["projectId"].template As<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__NodeProject_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__NodeProjects_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::NodeProjects Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::NodeProjects>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::NodeProjects res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::NodeProject>, std::vector<svetit::api::NodeProject>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::NodeProjects::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__NodeProjects_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Nodes_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Nodes Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Nodes>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Nodes res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Node>, std::vector<svetit::api::Node>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Nodes::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Nodes_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        static constexpr USERVER_NAMESPACE::utils::TrivialBiMap ksvetit__api__ParamType__Valuetype_Mapping =
            [](auto selector) {
            return selector().template Type<svetit::api::ParamType::Valuetype, std::string_view>()
                    .Case(svetit::api::ParamType::Valuetype::kInt, "int")
                    .Case(svetit::api::ParamType::Valuetype::kBool, "bool")
                    .Case(svetit::api::ParamType::Valuetype::kFloat, "float")
                    .Case(svetit::api::ParamType::Valuetype::kString, "string")
                    .Case(svetit::api::ParamType::Valuetype::kBytes, "bytes")
                    .Case(svetit::api::ParamType::Valuetype::kTime, "time")
                    .Case(svetit::api::ParamType::Valuetype::kRange, "range")
                    .Case(svetit::api::ParamType::Valuetype::kCombo, "combo");
        };
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__ParamType_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("id").Case("projectId").Case("parentId").Case("key").Case("name").Case("description").Case("valueType");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
	template<typename Value>
        svetit::api::ParamType::Valuetype Parse(Value val,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::ParamType::Valuetype>)
        {
            const auto value = val.template As<std::string>();
            const auto result = ksvetit__api__ParamType__Valuetype_Mapping.TryFindBySecond(value);
            if (result.has_value()) {
                return *result;
            }
            USERVER_NAMESPACE::chaotic::ThrowForValue(fmt::format("Invalid enum value ({}) for type svetit::api::ParamType::Valuetype", value), val);
        }
    

    

    
        
	template<typename Value>
        svetit::api::ParamType Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::ParamType>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::ParamType res;

            res.id =
                    value["id"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::ParamType::kIdMinimum>>>>
                    ();res.projectId =
                    value["projectId"].template As<std::optional<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>>
                    ();res.parentId =
                    value["parentId"].template As<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>
                    ();res.key =
                    value["key"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>>
                    ();res.name =
                    value["name"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>>
                    ();res.description =
                    value["description"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<512>>>
                    ();res.valueType =
                    value["valueType"].template As<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::ParamType::Valuetype>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__ParamType_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__ParamTypes_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::ParamTypes Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::ParamTypes>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::ParamTypes res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::ParamType>, std::vector<svetit::api::ParamType>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::ParamTypes::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__ParamTypes_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Plugin_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("id").Case("projectId").Case("name").Case("description").Case("key");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Plugin Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Plugin>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Plugin res;

            res.id =
                    value["id"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Plugin::kIdMinimum>>>>
                    ();res.projectId =
                    value["projectId"].template As<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>
                    ();res.name =
                    value["name"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>>
                    ();res.description =
                    value["description"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>
                    ();res.key =
                    value["key"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<128>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Plugin_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Plugins_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Plugins Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Plugins>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Plugins res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Plugin>, std::vector<svetit::api::Plugin>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Plugins::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Plugins_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        static constexpr USERVER_NAMESPACE::utils::TrivialBiMap ksvetit__api__Project__Sync_Mapping =
            [](auto selector) {
            return selector().template Type<svetit::api::Project::Sync, std::string_view>()
                    .Case(svetit::api::Project::Sync::kProjecttonode, "projectToNode")
                    .Case(svetit::api::Project::Sync::kNodetoproject, "nodeToProject");
        };
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Project_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("id").Case("spaceId").Case("key").Case("name").Case("description").Case("changedAt").Case("sync");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
	template<typename Value>
        svetit::api::Project::Sync Parse(Value val,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Project::Sync>)
        {
            const auto value = val.template As<std::string>();
            const auto result = ksvetit__api__Project__Sync_Mapping.TryFindBySecond(value);
            if (result.has_value()) {
                return *result;
            }
            USERVER_NAMESPACE::chaotic::ThrowForValue(fmt::format("Invalid enum value ({}) for type svetit::api::Project::Sync", value), val);
        }
    

    

    
        
	template<typename Value>
        svetit::api::Project Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Project>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Project res;

            res.id =
                    value["id"].template As<std::optional<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>>
                    ();res.spaceId =
                    value["spaceId"].template As<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>
                    ();res.key =
                    value["key"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<32>>>
                    ();res.name =
                    value["name"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>>
                    ();res.description =
                    value["description"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::string>>>
                    ();res.changedAt =
                    value["changedAt"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>>>
                    ();res.sync =
                    value["sync"].template As<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Project::Sync>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Project_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__ProjectParam_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("projectId").Case("paramId");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::ProjectParam Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::ProjectParam>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::ProjectParam res;

            res.projectId =
                    value["projectId"].template As<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>
                    ();res.paramId =
                    value["paramId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::ProjectParam::kParamidMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__ProjectParam_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__ProjectParams_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::ProjectParams Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::ProjectParams>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::ProjectParams res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::ProjectParam>, std::vector<svetit::api::ProjectParam>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::ProjectParams::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__ProjectParams_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Projects_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Projects Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Projects>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Projects res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Project>, std::vector<svetit::api::Project>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Projects::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Projects_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Role_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("id").Case("name");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Role Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Role>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Role res;

            res.id =
                    value["id"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::int32_t>>>
                    ();res.name =
                    value["name"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Role_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Roles_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Roles Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Roles>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Roles res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Role>, std::vector<svetit::api::Role>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Roles::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Roles_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__SaveTimer_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("id").Case("projectId").Case("intervalMsec");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::SaveTimer Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::SaveTimer>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::SaveTimer res;

            res.id =
                    value["id"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::SaveTimer::kIdMinimum>>>>
                    ();res.projectId =
                    value["projectId"].template As<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>
                    ();res.intervalMsec =
                    value["intervalMsec"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int32_t>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__SaveTimer_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__SaveTimers_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::SaveTimers Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::SaveTimers>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::SaveTimers res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::SaveTimer>, std::vector<svetit::api::SaveTimer>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::SaveTimers::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__SaveTimers_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Section_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("id").Case("projectId").Case("name");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Section Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Section>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Section res;

            res.id =
                    value["id"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Section::kIdMinimum>>>>
                    ();res.projectId =
                    value["projectId"].template As<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>
                    ();res.name =
                    value["name"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Section_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__SectionParam_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("sectionId").Case("paramId");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::SectionParam Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::SectionParam>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::SectionParam res;

            res.sectionId =
                    value["sectionId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::SectionParam::kSectionidMinimum>>>
                    ();res.paramId =
                    value["paramId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::SectionParam::kParamidMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__SectionParam_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__SectionParams_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::SectionParams Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::SectionParams>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::SectionParams res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::SectionParam>, std::vector<svetit::api::SectionParam>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::SectionParams::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__SectionParams_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Sections_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Sections Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Sections>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Sections res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Section>, std::vector<svetit::api::Section>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Sections::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Sections_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Space_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("id").Case("name").Case("key").Case("requestsAllowed").Case("createdAt");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Space Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Space>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Space res;

            res.id =
                    value["id"].template As<std::optional<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>>
                    ();res.name =
                    value["name"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>
                    ();res.key =
                    value["key"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>
                    ();res.requestsAllowed =
                    value["requestsAllowed"].template As<USERVER_NAMESPACE::chaotic::Primitive<bool>>
                    ();res.createdAt =
                    value["createdAt"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Space_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__SpaceParams_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("canCreate").Case("invitationSize");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::SpaceParams Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::SpaceParams>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::SpaceParams res;

            res.canCreate =
                    value["canCreate"].template As<USERVER_NAMESPACE::chaotic::Primitive<bool>>
                    ();res.invitationSize =
                    value["invitationSize"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int32_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::SpaceParams::kInvitationsizeMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__SpaceParams_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Spaces_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Spaces Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Spaces>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Spaces res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Space>, std::vector<svetit::api::Space>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Spaces::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Spaces_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Translation_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("id").Case("projectId").Case("lang").Case("key").Case("value");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Translation Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Translation>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Translation res;

            res.id =
                    value["id"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Translation::kIdMinimum>>>>
                    ();res.projectId =
                    value["projectId"].template As<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>
                    ();res.lang =
                    value["lang"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>>
                    ();res.key =
                    value["key"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>>
                    ();res.value =
                    value["value"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Translation_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Translations_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Translations Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Translations>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Translations res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Translation>, std::vector<svetit::api::Translation>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Translations::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Translations_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__User_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("userId").Case("spaceId").Case("isOwner").Case("joinedAt").Case("roleId");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::User Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::User>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::User res;

            res.userId =
                    value["userId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>
                    ();res.spaceId =
                    value["spaceId"].template As<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>
                    ();res.isOwner =
                    value["isOwner"].template As<USERVER_NAMESPACE::chaotic::Primitive<bool>>
                    ();res.joinedAt =
                    value["joinedAt"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>>
                    ();res.roleId =
                    value["roleId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int32_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::User::kRoleidMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__User_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__UserInfo_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("id").Case("displayName").Case("login").Case("firstname").Case("lastname").Case("email");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::UserInfo Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::UserInfo>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::UserInfo res;

            res.id =
                    value["id"].template As<std::optional<USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>>>
                    ();res.displayName =
                    value["displayName"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>
                    ();res.login =
                    value["login"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>
                    ();res.firstname =
                    value["firstname"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>
                    ();res.lastname =
                    value["lastname"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>
                    ();res.email =
                    value["email"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__UserInfo_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__UserInfos_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::UserInfos Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::UserInfos>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::UserInfos res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::UserInfo>, std::vector<svetit::api::UserInfo>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__UserInfos_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__Users_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::Users Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::Users>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::Users res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::User>, std::vector<svetit::api::User>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Users::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__Users_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__ValueView_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("id").Case("diTypeId").Case("value").Case("view");
                };

        
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::ValueView Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::ValueView>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::ValueView res;

            res.id =
                    value["id"].template As<std::optional<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::ValueView::kIdMinimum>>>>
                    ();res.diTypeId =
                    value["diTypeId"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::ValueView::kDitypeidMinimum>>>
                    ();res.value =
                    value["value"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>
                    ();res.view =
                    value["view"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__ValueView_PropertiesNames
                );
            
            return res;
        }
    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
        
            static constexpr USERVER_NAMESPACE::utils::TrivialSet
                ksvetit__api__ValueViews_PropertiesNames =
                [](auto selector) {
		    return selector().template Type<std::string_view>().Case("list").Case("total");
                };

        
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        
	template<typename Value>
        svetit::api::ValueViews Parse(Value value,
                         USERVER_NAMESPACE::formats::parse::To<svetit::api::ValueViews>)
        {
            value.CheckNotMissing();
            value.CheckObjectOrNull();

            svetit::api::ValueViews res;

            res.list =
                    value["list"].template As<USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::ValueView>, std::vector<svetit::api::ValueView>, USERVER_NAMESPACE::chaotic::MaxItems<100>>>
                    ();res.total =
                    value["total"].template As<USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::ValueViews::kTotalMinimum>>>
                    ();

            
            

            
                USERVER_NAMESPACE::chaotic::ValidateNoAdditionalProperties(
                    value, ksvetit__api__ValueViews_PropertiesNames
                );
            
            return res;
        }
    




    
        } //api
} //svetit

        
    
