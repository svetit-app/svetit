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


export interface DiType { 
    id?: number;
    measureId: number;
    saveTimerId: number;
    key: string;
    name: string;
    mode: DiType.ModeEnum;
    saveAlgorithm: DiType.SaveAlgorithmEnum;
}
export namespace DiType {
    export type ModeEnum = 'readonlyFlag' | 'readwriteFlag' | 'readonly' | 'readwrite' | 'file' | 'button' | 'videoStream';
    export const ModeEnum = {
        ReadonlyFlag: 'readonlyFlag' as ModeEnum,
        ReadwriteFlag: 'readwriteFlag' as ModeEnum,
        Readonly: 'readonly' as ModeEnum,
        Readwrite: 'readwrite' as ModeEnum,
        File: 'file' as ModeEnum,
        Button: 'button' as ModeEnum,
        VideoStream: 'videoStream' as ModeEnum
    };
    export type SaveAlgorithmEnum = 'off' | 'immediately' | 'byTimer' | 'byTimerOrImmediately';
    export const SaveAlgorithmEnum = {
        Off: 'off' as SaveAlgorithmEnum,
        Immediately: 'immediately' as SaveAlgorithmEnum,
        ByTimer: 'byTimer' as SaveAlgorithmEnum,
        ByTimerOrImmediately: 'byTimerOrImmediately' as SaveAlgorithmEnum
    };
}

