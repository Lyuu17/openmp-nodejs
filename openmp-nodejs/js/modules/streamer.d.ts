/*
 * open.mp nodejs impl
*/

/// <reference path="../types.d.ts"/>

declare enum StreamerItemType {
  STREAMER_TYPE_OBJECT = 0,
  STREAMER_TYPE_PICKUP,
  STREAMER_TYPE_CP,
  STREAMER_TYPE_RACE_CP,
  STREAMER_TYPE_MAP_ICON,
  STREAMER_TYPE_3D_TEXT_LABEL,
  STREAMER_TYPE_AREA,
  STREAMER_TYPE_ACTOR,
}

declare interface IStreamerObject {
  readonly id: number;
  position: Vector3;
  rotation: Vector3;

  destroy(): void;
}

declare interface IStreamerModule {
  getDynamicObject(): IStreamerObject;
  createDynamicObject(model: number, position: Vector3, rotation: Vector3, worldId?: number, interiorId?: number, player?: IPlayer, streamDistance?: number, areaId?: number): IStreamerObject;
  update(player: IPlayer, itemType: StreamerItemType): boolean;
  updateEx(player: IPlayer, itemType: StreamerItemType, position: Vector3, worldId?: number, interiorId?: number, compensatedTime?: number, freezePlayer?: boolean): boolean;
}

declare const streamer: IStreamerModule;
