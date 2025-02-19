/*
 * open.mp nodejs impl
*/

declare interface IResource {
  name: string;
  main: string;
  path: string;
}

declare var resource: IResource;

declare interface IEvent {
  preventDefault(): void;
}

declare type IEventHandler = (...args: any) => void;

declare class Quat {
  constructor(w: number, x: number, y: number, z: number);
  constructor(euler: Vector3);

  toEuler(): Vector3;

  w: number;
  x: number;
  y: number;
  z: number;
}

declare class Vector2 {
  constructor(x: number, y: number);

  x: number;
  y: number;
}

declare class Vector3 {
  constructor(x: number, y: number, z: number);

  x: number;
  y: number;
  z: number;
}

declare class Colour {
  r: number;
  g: number;
  b: number;
  a: number;
}

declare interface ISpawnInfo {
  team: number;
  skin: number;
  spawn: Vector3;
  angle: number;
  weapons: IWeaponSlots[];
}

declare interface IWeaponSlots {
  id: number;
  ammo: number;
}

declare interface ICheckpoint {
  position: Vector3;
  radius: number;
  enabled: boolean;
}

declare interface IPlayer {

  name: string;
  readonly id: number;
  position: Vector3;
  rotation: Quat;
  money: number;
  score: number;
  skin: number;
  interior: number;
  team: number;
  health: number;
  armour: number;
  weapon: number;
  weaponAmmo: number;
  drunkLevel: number;
  wantedLevel: number;
  controllable: boolean;
  spawnInfo: ISpawnInfo;
  readonly vehicle: IVehicle | undefined;
  readonly ip: string;
  ghostMode: boolean;
  specialAction: number;
  menu: number;
  dialog: number;
  colour: Colour;
  shopName: string;
  virtualWorld: number;
  checkpoint: ICheckpoint;


  kick(): void;
  ban(): void;
  giveMoney(money: number): number;
  giveWeapon(weapon: number, weaponAmmo: number): void;
  removeWeapon(money: number): void;
  setWeaponAmmo(weapon: number, weaponAmmo: number): void;
  getWeapons(): IWeaponSlots[];
  getWeaponSlot(): IWeaponSlots;
  resetWeapons(): void;
  spawn(): void;
  forceClassSelection(): void;
  hideDialog(): void;
  showDialog(style: number, title: string, body: string, button1: string, button2: string, callback: IEventHandler): void;
  setMapIcon(id: number, position: Vector3, type: number, color: Colour, style: number): void;
  unsetMapIcon(id: number): void;
  sendGameText(text: string, time: number, style: number): void;
}

declare interface IVehicle {

  readonly id: number;
  health: number;
  position: Vector3;
  rotation: Quat;
  readonly driver: IPlayer;
  readonly passengers: IPlayer[];
  plate: string;
  readonly model: number;
  colour: number[];
  virtualWorld: number;

  repair(): void;
  putPlayer(player: IPlayer, seat: number): void;
  addComponent(componentId: number): void;
  getComponentInSlot(slot: number): number;
  removeComponent(componentId: number): void;
}

declare interface IPickup {

  readonly id: number;
  type: number;
  model: number;
  position: Vector3;
  virtualWorld: number;

  destroy(): void;
  setPickupHiddenForPlayer(player: IPlayer, toggle: boolean): void;
  isPickupHiddenForPlayer(player: IPlayer): boolean;
}

declare interface IMenu {
  readonly id: number;
  readonly enabled: boolean;
  readonly position: Vector3;

  destroy(): void;
  disable(): void;
  showForPlayer(player: IPlayer): void;
  hideForPlayer(player: IPlayer): void;
  addCell(itemText: string, menuColumnId: number): void;
  setColumnHeader(header: string, menuColumnId: number): void;
  getCell(colId: number, menuRowId: number): string;
}
