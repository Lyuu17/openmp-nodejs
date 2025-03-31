/*
 * open.mp nodejs impl
*/

/// <reference path="../types.d.ts"/>

declare interface IStreamerObject {
  readonly id: number;
  position: Vector3;
  rotation: Vector3;

  destroy(): void;
}

declare interface IStreamerModule {
  getDynamicObject(): IStreamerObject;
  createDynamicObject(model: number, position: Vector3, rotation: Vector3, worldId?: number, interiorId?: number, player?: IPlayer, streamDistance?: number, areaId?: number): IStreamerObject;
}

declare const streamer: IStreamerModule;
