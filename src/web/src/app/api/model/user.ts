/**
 * Svetit MS Project
 * No description provided (generated by Openapi Generator https://github.com/openapitools/openapi-generator)
 *
 * The version of the OpenAPI document: 1.0.0
 * 
 *
 * NOTE: This class is auto generated by OpenAPI Generator (https://openapi-generator.tech).
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */


export interface User { 
    userId: string;
    spaceId: string;
    isOwner: boolean;
    joinedAt: number;
    role: User.RoleEnum;
}
export namespace User {
    export type RoleEnum = 'guest' | 'user' | 'admin';
    export const RoleEnum = {
        Guest: 'guest' as RoleEnum,
        User: 'user' as RoleEnum,
        Admin: 'admin' as RoleEnum
    };
}

