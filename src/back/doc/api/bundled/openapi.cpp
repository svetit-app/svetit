#include "openapi.hpp"

#include <userver/chaotic/type_bundle_cpp.hpp>

#include "openapi_parsers.ipp"























    
    
        
        namespace svetit {namespace api {
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::CcDi & lhs,const svetit::api::CcDi & rhs) {
            return
            lhs.ccId == rhs.ccId
                &&lhs.diId == rhs.diId
                &&
                true;
        }
    


    
    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::CcDi& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    

    
        CcDi Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::CcDi> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::CcDi& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["ccId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcDi::kCcidMinimum>>{
                            value.ccId
                        };
                
                    vb["diId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcDi::kDiidMinimum>>{
                            value.diId
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::CcDis & lhs,const svetit::api::CcDis & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::CcDis& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        CcDis Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::CcDis> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::CcDis& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::CcDi>, std::vector<svetit::api::CcDi>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcDis::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::CcModeType & lhs,const svetit::api::CcModeType & rhs) {
            return
            lhs.id == rhs.id
                &&lhs.ccTypeId == rhs.ccTypeId
                &&lhs.key == rhs.key
                &&lhs.name == rhs.name
                &&
                true;
        }
    


    
    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::CcModeType& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    

    
        CcModeType Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::CcModeType> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::CcModeType& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    if (value.id) {
                        vb["id"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcModeType::kIdMinimum>>{
                                *value.id
                            };
                    }
                
                    vb["ccTypeId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcModeType::kCctypeidMinimum>>{
                            value.ccTypeId
                        };
                
                    vb["key"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>{
                            value.key
                        };
                
                    vb["name"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>{
                            value.name
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::CcModeTypes & lhs,const svetit::api::CcModeTypes & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::CcModeTypes& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        CcModeTypes Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::CcModeTypes> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::CcModeTypes& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::CcModeType>, std::vector<svetit::api::CcModeType>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcModeTypes::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::CcParam & lhs,const svetit::api::CcParam & rhs) {
            return
            lhs.ccId == rhs.ccId
                &&lhs.paramId == rhs.paramId
                &&
                true;
        }
    


    
    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::CcParam& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    

    
        CcParam Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::CcParam> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::CcParam& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["ccId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcParam::kCcidMinimum>>{
                            value.ccId
                        };
                
                    vb["paramId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcParam::kParamidMinimum>>{
                            value.paramId
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::CcParams & lhs,const svetit::api::CcParams & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::CcParams& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        CcParams Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::CcParams> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::CcParams& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::CcParam>, std::vector<svetit::api::CcParam>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcParams::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::CcStatusCategory & lhs,const svetit::api::CcStatusCategory & rhs) {
            return
            lhs.id == rhs.id
                &&lhs.projectId == rhs.projectId
                &&lhs.key == rhs.key
                &&lhs.name == rhs.name
                &&lhs.color == rhs.color
                &&
                true;
        }
    


    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::CcStatusCategory& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    

    
        CcStatusCategory Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::CcStatusCategory> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::CcStatusCategory& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    if (value.id) {
                        vb["id"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcStatusCategory::kIdMinimum>>{
                                *value.id
                            };
                    }
                
                    vb["projectId"] = 
                        USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                            value.projectId
                        };
                
                    vb["key"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>{
                            value.key
                        };
                
                    vb["name"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>{
                            value.name
                        };
                
                    vb["color"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<16>>{
                            value.color
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::CcStatusCategories & lhs,const svetit::api::CcStatusCategories & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::CcStatusCategories& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        CcStatusCategories Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::CcStatusCategories> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::CcStatusCategories& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::CcStatusCategory>, std::vector<svetit::api::CcStatusCategory>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcStatusCategories::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::CcStatusType & lhs,const svetit::api::CcStatusType & rhs) {
            return
            lhs.id == rhs.id
                &&lhs.ccTypeId == rhs.ccTypeId
                &&lhs.categoryId == rhs.categoryId
                &&lhs.key == rhs.key
                &&lhs.text == rhs.text
                &&lhs.inform == rhs.inform
                &&
                true;
        }
    


    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::CcStatusType& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    

    
        CcStatusType Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::CcStatusType> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::CcStatusType& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    if (value.id) {
                        vb["id"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcStatusType::kIdMinimum>>{
                                *value.id
                            };
                    }
                
                    vb["ccTypeId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcStatusType::kCctypeidMinimum>>{
                            value.ccTypeId
                        };
                
                    vb["categoryId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcStatusType::kCategoryidMinimum>>{
                            value.categoryId
                        };
                
                    vb["key"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>{
                            value.key
                        };
                
                    vb["text"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<512>>{
                            value.text
                        };
                
                    vb["inform"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<bool>{
                            value.inform
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::CcStatusTypes & lhs,const svetit::api::CcStatusTypes & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::CcStatusTypes& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        CcStatusTypes Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::CcStatusTypes> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::CcStatusTypes& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::CcStatusType>, std::vector<svetit::api::CcStatusType>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcStatusTypes::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::CcType & lhs,const svetit::api::CcType & rhs) {
            return
            lhs.id == rhs.id
                &&lhs.projectId == rhs.projectId
                &&lhs.key == rhs.key
                &&lhs.name == rhs.name
                &&lhs.description == rhs.description
                &&
                true;
        }
    


    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::CcType& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    

    
        CcType Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::CcType> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::CcType& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    if (value.id) {
                        vb["id"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcType::kIdMinimum>>{
                                *value.id
                            };
                    }
                
                    vb["projectId"] = 
                        USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                            value.projectId
                        };
                
                    vb["key"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>{
                            value.key
                        };
                
                    vb["name"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>{
                            value.name
                        };
                
                    vb["description"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{
                            value.description
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::CcTypeDiType & lhs,const svetit::api::CcTypeDiType & rhs) {
            return
            lhs.ccTypeId == rhs.ccTypeId
                &&lhs.diTypeId == rhs.diTypeId
                &&
                true;
        }
    


    
    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::CcTypeDiType& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    

    
        CcTypeDiType Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::CcTypeDiType> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::CcTypeDiType& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["ccTypeId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcTypeDiType::kCctypeidMinimum>>{
                            value.ccTypeId
                        };
                
                    vb["diTypeId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcTypeDiType::kDitypeidMinimum>>{
                            value.diTypeId
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::CcTypeDiTypes & lhs,const svetit::api::CcTypeDiTypes & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::CcTypeDiTypes& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        CcTypeDiTypes Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::CcTypeDiTypes> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::CcTypeDiTypes& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::CcTypeDiType>, std::vector<svetit::api::CcTypeDiType>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcTypeDiTypes::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::CcTypeParam & lhs,const svetit::api::CcTypeParam & rhs) {
            return
            lhs.ccTypeId == rhs.ccTypeId
                &&lhs.paramId == rhs.paramId
                &&
                true;
        }
    


    
    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::CcTypeParam& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    

    
        CcTypeParam Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::CcTypeParam> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::CcTypeParam& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["ccTypeId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcTypeParam::kCctypeidMinimum>>{
                            value.ccTypeId
                        };
                
                    vb["paramId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcTypeParam::kParamidMinimum>>{
                            value.paramId
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::CcTypeParams & lhs,const svetit::api::CcTypeParams & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::CcTypeParams& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        CcTypeParams Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::CcTypeParams> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::CcTypeParams& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::CcTypeParam>, std::vector<svetit::api::CcTypeParam>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcTypeParams::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::CcTypes & lhs,const svetit::api::CcTypes & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::CcTypes& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        CcTypes Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::CcTypes> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::CcTypes& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::CcType>, std::vector<svetit::api::CcType>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::CcTypes::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Code & lhs,const svetit::api::Code & rhs) {
            return
            lhs.id == rhs.id
                &&lhs.projectId == rhs.projectId
                &&lhs.repositoryId == rhs.repositoryId
                &&lhs.commitHash == rhs.commitHash
                &&
                true;
        }
    


    
    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Code& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    

    
        Code Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Code> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Code& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    if (value.id) {
                        vb["id"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Code::kIdMinimum>>{
                                *value.id
                            };
                    }
                
                    vb["projectId"] = 
                        USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                            value.projectId
                        };
                
                    vb["repositoryId"] = 
                        USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                            value.repositoryId
                        };
                
                    vb["commitHash"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{
                            value.commitHash
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Codes & lhs,const svetit::api::Codes & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Codes& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        Codes Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Codes> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Codes& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Code>, std::vector<svetit::api::Code>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Codes::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::ControlCircuit & lhs,const svetit::api::ControlCircuit & rhs) {
            return
            lhs.id == rhs.id
                &&lhs.typeId == rhs.typeId
                &&lhs.sectionId == rhs.sectionId
                &&lhs.name == rhs.name
                &&
                true;
        }
    


    
    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::ControlCircuit& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    

    
        ControlCircuit Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::ControlCircuit> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::ControlCircuit& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    if (value.id) {
                        vb["id"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::ControlCircuit::kIdMinimum>>{
                                *value.id
                            };
                    }
                
                    vb["typeId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::ControlCircuit::kTypeidMinimum>>{
                            value.typeId
                        };
                
                    vb["sectionId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::ControlCircuit::kSectionidMinimum>>{
                            value.sectionId
                        };
                
                    vb["name"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>{
                            value.name
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::ControlCircuits & lhs,const svetit::api::ControlCircuits & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::ControlCircuits& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        ControlCircuits Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::ControlCircuits> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::ControlCircuits& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::ControlCircuit>, std::vector<svetit::api::ControlCircuit>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::ControlCircuits::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Device & lhs,const svetit::api::Device & rhs) {
            return
            lhs.id == rhs.id
                &&lhs.projectId == rhs.projectId
                &&lhs.pluginId == rhs.pluginId
                &&lhs.name == rhs.name
                &&lhs.checkIntervalMsec == rhs.checkIntervalMsec
                &&
                true;
        }
    


    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Device& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    

    
        Device Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Device> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Device& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    if (value.id) {
                        vb["id"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Device::kIdMinimum>>{
                                *value.id
                            };
                    }
                
                    vb["projectId"] = 
                        USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                            value.projectId
                        };
                
                    vb["pluginId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Device::kPluginidMinimum>>{
                            value.pluginId
                        };
                
                    vb["name"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>{
                            value.name
                        };
                
                    vb["checkIntervalMsec"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int32_t>{
                            value.checkIntervalMsec
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::DeviceItem & lhs,const svetit::api::DeviceItem & rhs) {
            return
            lhs.id == rhs.id
                &&lhs.deviceId == rhs.deviceId
                &&lhs.typeId == rhs.typeId
                &&lhs.name == rhs.name
                &&
                true;
        }
    


    
    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::DeviceItem& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    

    
        DeviceItem Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::DeviceItem> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::DeviceItem& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    if (value.id) {
                        vb["id"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DeviceItem::kIdMinimum>>{
                                *value.id
                            };
                    }
                
                    vb["deviceId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DeviceItem::kDeviceidMinimum>>{
                            value.deviceId
                        };
                
                    vb["typeId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DeviceItem::kTypeidMinimum>>{
                            value.typeId
                        };
                
                    vb["name"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>{
                            value.name
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::DeviceItems & lhs,const svetit::api::DeviceItems & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::DeviceItems& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        DeviceItems Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::DeviceItems> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::DeviceItems& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::DeviceItem>, std::vector<svetit::api::DeviceItem>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DeviceItems::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::DevicePluginParam & lhs,const svetit::api::DevicePluginParam & rhs) {
            return
            lhs.deviceId == rhs.deviceId
                &&lhs.paramId == rhs.paramId
                &&
                true;
        }
    


    
    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::DevicePluginParam& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    

    
        DevicePluginParam Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::DevicePluginParam> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::DevicePluginParam& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["deviceId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DevicePluginParam::kDeviceidMinimum>>{
                            value.deviceId
                        };
                
                    vb["paramId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DevicePluginParam::kParamidMinimum>>{
                            value.paramId
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::DevicePluginParams & lhs,const svetit::api::DevicePluginParams & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::DevicePluginParams& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        DevicePluginParams Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::DevicePluginParams> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::DevicePluginParams& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::DevicePluginParam>, std::vector<svetit::api::DevicePluginParam>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DevicePluginParams::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Devices & lhs,const svetit::api::Devices & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Devices& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        Devices Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Devices> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Devices& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Device>, std::vector<svetit::api::Device>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Devices::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::DiPluginParam & lhs,const svetit::api::DiPluginParam & rhs) {
            return
            lhs.diTypeId == rhs.diTypeId
                &&lhs.paramId == rhs.paramId
                &&
                true;
        }
    


    
    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::DiPluginParam& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    

    
        DiPluginParam Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::DiPluginParam> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::DiPluginParam& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["diTypeId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DiPluginParam::kDitypeidMinimum>>{
                            value.diTypeId
                        };
                
                    vb["paramId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DiPluginParam::kParamidMinimum>>{
                            value.paramId
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::DiPluginParams & lhs,const svetit::api::DiPluginParams & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::DiPluginParams& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        DiPluginParams Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::DiPluginParams> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::DiPluginParams& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::DiPluginParam>, std::vector<svetit::api::DiPluginParam>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DiPluginParams::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::DiType & lhs,const svetit::api::DiType & rhs) {
            return
            lhs.id == rhs.id
                &&lhs.measureId == rhs.measureId
                &&lhs.saveTimerId == rhs.saveTimerId
                &&lhs.key == rhs.key
                &&lhs.name == rhs.name
                &&lhs.mode == rhs.mode
                &&lhs.saveAlgorithm == rhs.saveAlgorithm
                &&
                true;
        }
    


    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    
        USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::DiType::Mode& value)
        {
            return lh << ToString(value);
        }
    

    
    

    
        USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::DiType::Savealgorithm& value)
        {
            return lh << ToString(value);
        }
    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::DiType& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    
        DiType::Mode Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType::Mode> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    
    
        
    

    
        DiType::Savealgorithm Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType::Savealgorithm> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
        DiType Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    
        
            svetit::api::DiType::Mode FromString(std::string_view value,
                            USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType::Mode>)
            {
                const auto result = ksvetit__api__DiType__Mode_Mapping.TryFindBySecond(value);
                if (result.has_value()) {
                    return *result;
                }
                throw std::runtime_error(fmt::format("Invalid enum value ({}) for type svetit::api::DiType::Mode", value));
            }

            svetit::api::DiType::Mode Parse(std::string_view value,
                            USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType::Mode> to)
            {
                return FromString(value, to);
            }
        
    

    
    

    
        
            svetit::api::DiType::Savealgorithm FromString(std::string_view value,
                            USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType::Savealgorithm>)
            {
                const auto result = ksvetit__api__DiType__Savealgorithm_Mapping.TryFindBySecond(value);
                if (result.has_value()) {
                    return *result;
                }
                throw std::runtime_error(fmt::format("Invalid enum value ({}) for type svetit::api::DiType::Savealgorithm", value));
            }

            svetit::api::DiType::Savealgorithm Parse(std::string_view value,
                            USERVER_NAMESPACE::formats::parse::To<svetit::api::DiType::Savealgorithm> to)
            {
                return FromString(value, to);
            }
        
    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            const svetit::api::DiType::Mode& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            return USERVER_NAMESPACE::formats::json::ValueBuilder(ToString(value)).ExtractValue();
        }
    

    
    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            const svetit::api::DiType::Savealgorithm& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            return USERVER_NAMESPACE::formats::json::ValueBuilder(ToString(value)).ExtractValue();
        }
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::DiType& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    if (value.id) {
                        vb["id"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DiType::kIdMinimum>>{
                                *value.id
                            };
                    }
                
                    vb["measureId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DiType::kMeasureidMinimum>>{
                            value.measureId
                        };
                
                    vb["saveTimerId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DiType::kSavetimeridMinimum>>{
                            value.saveTimerId
                        };
                
                    vb["key"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>{
                            value.key
                        };
                
                    vb["name"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>{
                            value.name
                        };
                
                    vb["mode"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<svetit::api::DiType::Mode>{
                            value.mode
                        };
                
                    vb["saveAlgorithm"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<svetit::api::DiType::Savealgorithm>{
                            value.saveAlgorithm
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    
        std::string ToString(svetit::api::DiType::Mode value) {
            const auto result = ksvetit__api__DiType__Mode_Mapping.TryFindByFirst(value);
            if (result.has_value()) {
                return std::string{*result};
            }
            throw std::runtime_error("Bad enum value");
        }
    

    
    

    
        std::string ToString(svetit::api::DiType::Savealgorithm value) {
            const auto result = ksvetit__api__DiType__Savealgorithm_Mapping.TryFindByFirst(value);
            if (result.has_value()) {
                return std::string{*result};
            }
            throw std::runtime_error("Bad enum value");
        }
    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::DiTypes & lhs,const svetit::api::DiTypes & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::DiTypes& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        DiTypes Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::DiTypes> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::DiTypes& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::DiType>, std::vector<svetit::api::DiType>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::DiTypes::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Error & lhs,const svetit::api::Error & rhs) {
            return
            lhs.code == rhs.code
                &&lhs.message == rhs.message
                &&
                true;
        }
    


    
    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Error& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    

    
        Error Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Error> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Error& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["code"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int32_t>{
                            value.code
                        };
                
                    vb["message"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{
                            value.message
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Group & lhs,const svetit::api::Group & rhs) {
            return
            lhs.id == rhs.id
                &&lhs.name == rhs.name
                &&lhs.description == rhs.description
                &&lhs.spaceId == rhs.spaceId
                &&
                true;
        }
    


    
    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Group& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    

    
        Group Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Group> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Group& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    if (value.id) {
                        vb["id"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int32_t>{
                                *value.id
                            };
                    }
                
                    vb["name"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>{
                            value.name
                        };
                
                    vb["description"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string>{
                            value.description
                        };
                
                    if (value.spaceId) {
                        vb["spaceId"] = 
                            USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                                *value.spaceId
                            };
                    }
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::GroupUser & lhs,const svetit::api::GroupUser & rhs) {
            return
            lhs.nodeId == rhs.nodeId
                &&lhs.userId == rhs.userId
                &&
                true;
        }
    


    
    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::GroupUser& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    

    
        GroupUser Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::GroupUser> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::GroupUser& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["nodeId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int32_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::GroupUser::kNodeidMinimum>>{
                            value.nodeId
                        };
                
                    vb["userId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<40>>{
                            value.userId
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::GroupUsers & lhs,const svetit::api::GroupUsers & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::GroupUsers& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        GroupUsers Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::GroupUsers> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::GroupUsers& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::GroupUser>, std::vector<svetit::api::GroupUser>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::GroupUsers::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Groups & lhs,const svetit::api::Groups & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Groups& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        Groups Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Groups> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Groups& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Group>, std::vector<svetit::api::Group>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Groups::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Invitation & lhs,const svetit::api::Invitation & rhs) {
            return
            lhs.id == rhs.id
                &&lhs.spaceId == rhs.spaceId
                &&lhs.creatorId == rhs.creatorId
                &&lhs.userId == rhs.userId
                &&lhs.roleId == rhs.roleId
                &&lhs.createdAt == rhs.createdAt
                &&
                true;
        }
    


    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Invitation& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    

    
        Invitation Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Invitation> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Invitation& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    if (value.id) {
                        vb["id"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int32_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Invitation::kIdMinimum>>{
                                *value.id
                            };
                    }
                
                    vb["spaceId"] = 
                        USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                            value.spaceId
                        };
                
                    if (value.creatorId) {
                        vb["creatorId"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{
                                *value.creatorId
                            };
                    }
                
                    vb["userId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{
                            value.userId
                        };
                
                    if (value.roleId) {
                        vb["roleId"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int32_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Invitation::kRoleidMinimum>>{
                                *value.roleId
                            };
                    }
                
                    if (value.createdAt) {
                        vb["createdAt"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>{
                                *value.createdAt
                            };
                    }
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    

    
        bool operator==(const svetit::api::InvitationRole & lhs,const svetit::api::InvitationRole & rhs) {
            return
            lhs.roleId == rhs.roleId
                &&
                true;
        }
    


    
    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::InvitationRole& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    

    
        InvitationRole Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::InvitationRole> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    

    
        
    


    
        
    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::InvitationRole& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["roleId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int32_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::InvitationRole::kRoleidMinimum>>{
                            value.roleId
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Invitations & lhs,const svetit::api::Invitations & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Invitations& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        Invitations Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Invitations> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Invitations& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Invitation>, std::vector<svetit::api::Invitation>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Invitations::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Link & lhs,const svetit::api::Link & rhs) {
            return
            lhs.id == rhs.id
                &&lhs.spaceId == rhs.spaceId
                &&lhs.creatorId == rhs.creatorId
                &&lhs.name == rhs.name
                &&lhs.createdAt == rhs.createdAt
                &&lhs.expiredAt == rhs.expiredAt
                &&
                true;
        }
    


    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Link& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    

    
        Link Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Link> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Link& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    if (value.id) {
                        vb["id"] = 
                            USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                                *value.id
                            };
                    }
                
                    vb["spaceId"] = 
                        USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                            value.spaceId
                        };
                
                    if (value.creatorId) {
                        vb["creatorId"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{
                                *value.creatorId
                            };
                    }
                
                    vb["name"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{
                            value.name
                        };
                
                    if (value.createdAt) {
                        vb["createdAt"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>{
                                *value.createdAt
                            };
                    }
                
                    vb["expiredAt"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>{
                            value.expiredAt
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Links & lhs,const svetit::api::Links & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Links& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        Links Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Links> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Links& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Link>, std::vector<svetit::api::Link>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Links::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Measure & lhs,const svetit::api::Measure & rhs) {
            return
            lhs.id == rhs.id
                &&lhs.projectId == rhs.projectId
                &&lhs.name == rhs.name
                &&
                true;
        }
    


    
    
    

    

    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Measure& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    

    
        Measure Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Measure> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Measure& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    if (value.id) {
                        vb["id"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Measure::kIdMinimum>>{
                                *value.id
                            };
                    }
                
                    vb["projectId"] = 
                        USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                            value.projectId
                        };
                
                    vb["name"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<10>>{
                            value.name
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Measures & lhs,const svetit::api::Measures & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Measures& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        Measures Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Measures> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Measures& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Measure>, std::vector<svetit::api::Measure>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Measures::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Node & lhs,const svetit::api::Node & rhs) {
            return
            lhs.id == rhs.id
                &&lhs.spaceId == rhs.spaceId
                &&lhs.name == rhs.name
                &&lhs.description == rhs.description
                &&lhs.latitude == rhs.latitude
                &&lhs.longitude == rhs.longitude
                &&lhs.createdAt == rhs.createdAt
                &&
                true;
        }
    


    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Node& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    

    
        Node Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Node> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Node& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    if (value.id) {
                        vb["id"] = 
                            USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                                *value.id
                            };
                    }
                
                    if (value.spaceId) {
                        vb["spaceId"] = 
                            USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                                *value.spaceId
                            };
                    }
                
                    vb["name"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>{
                            value.name
                        };
                
                    vb["description"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string>{
                            value.description
                        };
                
                    vb["latitude"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<double>{
                            value.latitude
                        };
                
                    vb["longitude"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<double>{
                            value.longitude
                        };
                
                    if (value.createdAt) {
                        vb["createdAt"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>{
                                *value.createdAt
                            };
                    }
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::NodeGroup & lhs,const svetit::api::NodeGroup & rhs) {
            return
            lhs.nodeId == rhs.nodeId
                &&lhs.groupId == rhs.groupId
                &&
                true;
        }
    


    
    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::NodeGroup& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    

    
        NodeGroup Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::NodeGroup> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::NodeGroup& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["nodeId"] = 
                        USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                            value.nodeId
                        };
                
                    vb["groupId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int32_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::NodeGroup::kGroupidMinimum>>{
                            value.groupId
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::NodeGroups & lhs,const svetit::api::NodeGroups & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::NodeGroups& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        NodeGroups Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::NodeGroups> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::NodeGroups& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::NodeGroup>, std::vector<svetit::api::NodeGroup>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::NodeGroups::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::NodeProject & lhs,const svetit::api::NodeProject & rhs) {
            return
            lhs.nodeId == rhs.nodeId
                &&lhs.projectId == rhs.projectId
                &&
                true;
        }
    


    
    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::NodeProject& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    

    
        NodeProject Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::NodeProject> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::NodeProject& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["nodeId"] = 
                        USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                            value.nodeId
                        };
                
                    vb["projectId"] = 
                        USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                            value.projectId
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::NodeProjects & lhs,const svetit::api::NodeProjects & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::NodeProjects& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        NodeProjects Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::NodeProjects> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::NodeProjects& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::NodeProject>, std::vector<svetit::api::NodeProject>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::NodeProjects::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Nodes & lhs,const svetit::api::Nodes & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Nodes& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        Nodes Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Nodes> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Nodes& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Node>, std::vector<svetit::api::Node>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Nodes::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::ParamType & lhs,const svetit::api::ParamType & rhs) {
            return
            lhs.id == rhs.id
                &&lhs.projectId == rhs.projectId
                &&lhs.parentId == rhs.parentId
                &&lhs.key == rhs.key
                &&lhs.name == rhs.name
                &&lhs.description == rhs.description
                &&lhs.valueType == rhs.valueType
                &&
                true;
        }
    


    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    
        USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::ParamType::Valuetype& value)
        {
            return lh << ToString(value);
        }
    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::ParamType& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    
        ParamType::Valuetype Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::ParamType::Valuetype> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
        ParamType Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::ParamType> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    
        
            svetit::api::ParamType::Valuetype FromString(std::string_view value,
                            USERVER_NAMESPACE::formats::parse::To<svetit::api::ParamType::Valuetype>)
            {
                const auto result = ksvetit__api__ParamType__Valuetype_Mapping.TryFindBySecond(value);
                if (result.has_value()) {
                    return *result;
                }
                throw std::runtime_error(fmt::format("Invalid enum value ({}) for type svetit::api::ParamType::Valuetype", value));
            }

            svetit::api::ParamType::Valuetype Parse(std::string_view value,
                            USERVER_NAMESPACE::formats::parse::To<svetit::api::ParamType::Valuetype> to)
            {
                return FromString(value, to);
            }
        
    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            const svetit::api::ParamType::Valuetype& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            return USERVER_NAMESPACE::formats::json::ValueBuilder(ToString(value)).ExtractValue();
        }
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::ParamType& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    if (value.id) {
                        vb["id"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::ParamType::kIdMinimum>>{
                                *value.id
                            };
                    }
                
                    if (value.projectId) {
                        vb["projectId"] = 
                            USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                                *value.projectId
                            };
                    }
                
                    if (value.parentId) {
                        vb["parentId"] = 
                            USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                                *value.parentId
                            };
                    }
                
                    vb["key"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>{
                            value.key
                        };
                
                    vb["name"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>{
                            value.name
                        };
                
                    vb["description"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<512>>{
                            value.description
                        };
                
                    vb["valueType"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<svetit::api::ParamType::Valuetype>{
                            value.valueType
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    
        std::string ToString(svetit::api::ParamType::Valuetype value) {
            const auto result = ksvetit__api__ParamType__Valuetype_Mapping.TryFindByFirst(value);
            if (result.has_value()) {
                return std::string{*result};
            }
            throw std::runtime_error("Bad enum value");
        }
    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::ParamTypes & lhs,const svetit::api::ParamTypes & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::ParamTypes& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        ParamTypes Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::ParamTypes> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::ParamTypes& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::ParamType>, std::vector<svetit::api::ParamType>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::ParamTypes::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Plugin & lhs,const svetit::api::Plugin & rhs) {
            return
            lhs.id == rhs.id
                &&lhs.projectId == rhs.projectId
                &&lhs.name == rhs.name
                &&lhs.description == rhs.description
                &&lhs.key == rhs.key
                &&
                true;
        }
    


    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Plugin& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    

    
        Plugin Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Plugin> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Plugin& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    if (value.id) {
                        vb["id"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Plugin::kIdMinimum>>{
                                *value.id
                            };
                    }
                
                    vb["projectId"] = 
                        USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                            value.projectId
                        };
                
                    vb["name"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>{
                            value.name
                        };
                
                    vb["description"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{
                            value.description
                        };
                
                    vb["key"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<128>>{
                            value.key
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Plugins & lhs,const svetit::api::Plugins & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Plugins& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        Plugins Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Plugins> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Plugins& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Plugin>, std::vector<svetit::api::Plugin>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Plugins::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Project & lhs,const svetit::api::Project & rhs) {
            return
            lhs.id == rhs.id
                &&lhs.spaceId == rhs.spaceId
                &&lhs.key == rhs.key
                &&lhs.name == rhs.name
                &&lhs.description == rhs.description
                &&lhs.changedAt == rhs.changedAt
                &&lhs.sync == rhs.sync
                &&
                true;
        }
    


    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    
        USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Project::Sync& value)
        {
            return lh << ToString(value);
        }
    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Project& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    
        Project::Sync Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Project::Sync> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
        Project Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Project> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    
        
            svetit::api::Project::Sync FromString(std::string_view value,
                            USERVER_NAMESPACE::formats::parse::To<svetit::api::Project::Sync>)
            {
                const auto result = ksvetit__api__Project__Sync_Mapping.TryFindBySecond(value);
                if (result.has_value()) {
                    return *result;
                }
                throw std::runtime_error(fmt::format("Invalid enum value ({}) for type svetit::api::Project::Sync", value));
            }

            svetit::api::Project::Sync Parse(std::string_view value,
                            USERVER_NAMESPACE::formats::parse::To<svetit::api::Project::Sync> to)
            {
                return FromString(value, to);
            }
        
    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            const svetit::api::Project::Sync& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            return USERVER_NAMESPACE::formats::json::ValueBuilder(ToString(value)).ExtractValue();
        }
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Project& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    if (value.id) {
                        vb["id"] = 
                            USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                                *value.id
                            };
                    }
                
                    vb["spaceId"] = 
                        USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                            value.spaceId
                        };
                
                    vb["key"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<32>>{
                            value.key
                        };
                
                    vb["name"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>{
                            value.name
                        };
                
                    if (value.description) {
                        vb["description"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::string>{
                                *value.description
                            };
                    }
                
                    if (value.changedAt) {
                        vb["changedAt"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>{
                                *value.changedAt
                            };
                    }
                
                    vb["sync"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Project::Sync>{
                            value.sync
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    
        std::string ToString(svetit::api::Project::Sync value) {
            const auto result = ksvetit__api__Project__Sync_Mapping.TryFindByFirst(value);
            if (result.has_value()) {
                return std::string{*result};
            }
            throw std::runtime_error("Bad enum value");
        }
    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::ProjectParam & lhs,const svetit::api::ProjectParam & rhs) {
            return
            lhs.projectId == rhs.projectId
                &&lhs.paramId == rhs.paramId
                &&
                true;
        }
    


    
    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::ProjectParam& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    

    
        ProjectParam Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::ProjectParam> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::ProjectParam& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["projectId"] = 
                        USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                            value.projectId
                        };
                
                    vb["paramId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::ProjectParam::kParamidMinimum>>{
                            value.paramId
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::ProjectParams & lhs,const svetit::api::ProjectParams & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::ProjectParams& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        ProjectParams Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::ProjectParams> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::ProjectParams& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::ProjectParam>, std::vector<svetit::api::ProjectParam>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::ProjectParams::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Projects & lhs,const svetit::api::Projects & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Projects& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        Projects Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Projects> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Projects& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Project>, std::vector<svetit::api::Project>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Projects::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Role & lhs,const svetit::api::Role & rhs) {
            return
            lhs.id == rhs.id
                &&lhs.name == rhs.name
                &&
                true;
        }
    


    
    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Role& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    

    
        Role Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Role> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Role& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    if (value.id) {
                        vb["id"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int32_t>{
                                *value.id
                            };
                    }
                
                    vb["name"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>{
                            value.name
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Roles & lhs,const svetit::api::Roles & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Roles& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        Roles Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Roles> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Roles& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Role>, std::vector<svetit::api::Role>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Roles::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::SaveTimer & lhs,const svetit::api::SaveTimer & rhs) {
            return
            lhs.id == rhs.id
                &&lhs.projectId == rhs.projectId
                &&lhs.intervalMsec == rhs.intervalMsec
                &&
                true;
        }
    


    
    
    

    

    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::SaveTimer& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    

    
        SaveTimer Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::SaveTimer> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::SaveTimer& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    if (value.id) {
                        vb["id"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::SaveTimer::kIdMinimum>>{
                                *value.id
                            };
                    }
                
                    vb["projectId"] = 
                        USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                            value.projectId
                        };
                
                    vb["intervalMsec"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int32_t>{
                            value.intervalMsec
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::SaveTimers & lhs,const svetit::api::SaveTimers & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::SaveTimers& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        SaveTimers Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::SaveTimers> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::SaveTimers& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::SaveTimer>, std::vector<svetit::api::SaveTimer>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::SaveTimers::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Section & lhs,const svetit::api::Section & rhs) {
            return
            lhs.id == rhs.id
                &&lhs.projectId == rhs.projectId
                &&lhs.name == rhs.name
                &&
                true;
        }
    


    
    
    

    

    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Section& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    

    
        Section Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Section> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Section& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    if (value.id) {
                        vb["id"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Section::kIdMinimum>>{
                                *value.id
                            };
                    }
                
                    vb["projectId"] = 
                        USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                            value.projectId
                        };
                
                    vb["name"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>{
                            value.name
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::SectionParam & lhs,const svetit::api::SectionParam & rhs) {
            return
            lhs.sectionId == rhs.sectionId
                &&lhs.paramId == rhs.paramId
                &&
                true;
        }
    


    
    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::SectionParam& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    

    
        SectionParam Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::SectionParam> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::SectionParam& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["sectionId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::SectionParam::kSectionidMinimum>>{
                            value.sectionId
                        };
                
                    vb["paramId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::SectionParam::kParamidMinimum>>{
                            value.paramId
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::SectionParams & lhs,const svetit::api::SectionParams & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::SectionParams& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        SectionParams Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::SectionParams> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::SectionParams& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::SectionParam>, std::vector<svetit::api::SectionParam>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::SectionParams::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Sections & lhs,const svetit::api::Sections & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Sections& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        Sections Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Sections> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Sections& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Section>, std::vector<svetit::api::Section>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Sections::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Space & lhs,const svetit::api::Space & rhs) {
            return
            lhs.id == rhs.id
                &&lhs.name == rhs.name
                &&lhs.key == rhs.key
                &&lhs.requestsAllowed == rhs.requestsAllowed
                &&lhs.createdAt == rhs.createdAt
                &&
                true;
        }
    


    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Space& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    

    
        Space Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Space> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Space& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    if (value.id) {
                        vb["id"] = 
                            USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                                *value.id
                            };
                    }
                
                    vb["name"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{
                            value.name
                        };
                
                    vb["key"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{
                            value.key
                        };
                
                    vb["requestsAllowed"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<bool>{
                            value.requestsAllowed
                        };
                
                    if (value.createdAt) {
                        vb["createdAt"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>{
                                *value.createdAt
                            };
                    }
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::SpaceParams & lhs,const svetit::api::SpaceParams & rhs) {
            return
            lhs.canCreate == rhs.canCreate
                &&lhs.invitationSize == rhs.invitationSize
                &&
                true;
        }
    


    
    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::SpaceParams& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    

    
        SpaceParams Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::SpaceParams> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::SpaceParams& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["canCreate"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<bool>{
                            value.canCreate
                        };
                
                    vb["invitationSize"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int32_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::SpaceParams::kInvitationsizeMinimum>>{
                            value.invitationSize
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Spaces & lhs,const svetit::api::Spaces & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Spaces& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        Spaces Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Spaces> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Spaces& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Space>, std::vector<svetit::api::Space>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Spaces::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Translation & lhs,const svetit::api::Translation & rhs) {
            return
            lhs.id == rhs.id
                &&lhs.projectId == rhs.projectId
                &&lhs.lang == rhs.lang
                &&lhs.key == rhs.key
                &&lhs.value == rhs.value
                &&
                true;
        }
    


    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Translation& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    

    
        Translation Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Translation> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Translation& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    if (value.id) {
                        vb["id"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Translation::kIdMinimum>>{
                                *value.id
                            };
                    }
                
                    vb["projectId"] = 
                        USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                            value.projectId
                        };
                
                    vb["lang"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>{
                            value.lang
                        };
                
                    vb["key"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>, USERVER_NAMESPACE::chaotic::MaxLength<64>>{
                            value.key
                        };
                
                    vb["value"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{
                            value.value
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Translations & lhs,const svetit::api::Translations & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Translations& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        Translations Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Translations> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Translations& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::Translation>, std::vector<svetit::api::Translation>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Translations::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::User & lhs,const svetit::api::User & rhs) {
            return
            lhs.userId == rhs.userId
                &&lhs.spaceId == rhs.spaceId
                &&lhs.isOwner == rhs.isOwner
                &&lhs.joinedAt == rhs.joinedAt
                &&lhs.roleId == rhs.roleId
                &&
                true;
        }
    


    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::User& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    

    
        User Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::User> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::User& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["userId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{
                            value.userId
                        };
                
                    vb["spaceId"] = 
                        USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                            value.spaceId
                        };
                
                    vb["isOwner"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<bool>{
                            value.isOwner
                        };
                
                    vb["joinedAt"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t>{
                            value.joinedAt
                        };
                
                    vb["roleId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int32_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::User::kRoleidMinimum>>{
                            value.roleId
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::UserInfo & lhs,const svetit::api::UserInfo & rhs) {
            return
            lhs.id == rhs.id
                &&lhs.displayName == rhs.displayName
                &&lhs.login == rhs.login
                &&lhs.firstname == rhs.firstname
                &&lhs.lastname == rhs.lastname
                &&lhs.email == rhs.email
                &&
                true;
        }
    


    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::UserInfo& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    

    
        UserInfo Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::UserInfo> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::UserInfo& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    if (value.id) {
                        vb["id"] = 
                            USERVER_NAMESPACE::chaotic::WithType<USERVER_NAMESPACE::chaotic::Primitive<std::string>, boost::uuids::uuid>{
                                *value.id
                            };
                    }
                
                    vb["displayName"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{
                            value.displayName
                        };
                
                    vb["login"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{
                            value.login
                        };
                
                    vb["firstname"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{
                            value.firstname
                        };
                
                    vb["lastname"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{
                            value.lastname
                        };
                
                    vb["email"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{
                            value.email
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    

    
        bool operator==(const svetit::api::UserInfos & lhs,const svetit::api::UserInfos & rhs) {
            return
            lhs.list == rhs.list
                &&
                true;
        }
    


    
    
    
    

    

    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::UserInfos& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    

    
        UserInfos Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::UserInfos> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::UserInfos& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::UserInfo>, std::vector<svetit::api::UserInfo>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::Users & lhs,const svetit::api::Users & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Users& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        Users Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::Users> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::Users& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::User>, std::vector<svetit::api::User>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::Users::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    


    
    


    
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::ValueView & lhs,const svetit::api::ValueView & rhs) {
            return
            lhs.id == rhs.id
                &&lhs.diTypeId == rhs.diTypeId
                &&lhs.value == rhs.value
                &&lhs.view == rhs.view
                &&
                true;
        }
    


    
    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::ValueView& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    
    
        
    

    

    

    

    
        ValueView Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::ValueView> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    

    
        
    


    
        
    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::ValueView& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    if (value.id) {
                        vb["id"] = 
                            USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::ValueView::kIdMinimum>>{
                                *value.id
                            };
                    }
                
                    vb["diTypeId"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::ValueView::kDitypeidMinimum>>{
                            value.diTypeId
                        };
                
                    vb["value"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{
                            value.value
                        };
                
                    vb["view"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::string, USERVER_NAMESPACE::chaotic::MinLength<1>>{
                            value.view
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    

    

    
    

    

    
    

    

    
    

    

    

    


    
    


    
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        bool operator==(const svetit::api::ValueViews & lhs,const svetit::api::ValueViews & rhs) {
            return
            lhs.list == rhs.list
                &&lhs.total == rhs.total
                &&
                true;
        }
    


    
    
    
    

    

    

    

    
    

    

    

    
        
            USERVER_NAMESPACE::logging::LogHelper& operator<<(USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::ValueViews& value)
            {
                return lh << ToString(USERVER_NAMESPACE::formats::json::ValueBuilder(value).ExtractValue());
            }
        
    


    
        
    
        
    
    
        
    
    
        
    

    

    

    

    

    

    
    
        
    

    

    

    

    
        ValueViews Parse(USERVER_NAMESPACE::formats::json::Value json,
                                                 USERVER_NAMESPACE::formats::parse::To<svetit::api::ValueViews> to)
    {
        return Parse<USERVER_NAMESPACE::formats::json::Value>(json, to);
    }
    

    

    

    
    
    
    

    

    

    

    
    

    

    

    
        
    


    
        
    
    
    

    
        
    

    

    
        
    

    
    

    
        
    

    

    
        USERVER_NAMESPACE::formats::json::Value Serialize(
            [[maybe_unused]] const svetit::api::ValueViews& value,
            USERVER_NAMESPACE::formats::serialize::To<USERVER_NAMESPACE::formats::json::Value>
        )
        {
            USERVER_NAMESPACE::formats::json::ValueBuilder vb
                    = USERVER_NAMESPACE::formats::common::Type::kObject;

            
                    vb["list"] = 
                        USERVER_NAMESPACE::chaotic::Array<USERVER_NAMESPACE::chaotic::Primitive<svetit::api::ValueView>, std::vector<svetit::api::ValueView>, USERVER_NAMESPACE::chaotic::MaxItems<100>>{
                            value.list
                        };
                
                    vb["total"] = 
                        USERVER_NAMESPACE::chaotic::Primitive<std::int64_t, USERVER_NAMESPACE::chaotic::Minimum<svetit::api::ValueViews::kTotalMinimum>>{
                            value.total
                        };
                

            
            return vb.ExtractValue();
        }
    

    

    
    
    
    

    

    

    

    
    

    

    

    




    
        } //api
} //svetit

        
    
