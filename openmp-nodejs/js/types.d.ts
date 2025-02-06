/*
 * open.mp nodejs impl
*/

declare interface Vector3 {
  x: number;
  y: number;
  z: number;
}

declare interface ISpawnInfo {
  team: number;
  skin: number;
  spawn: number;
  angle: number;
  weapons: IWeaponSlots[];
}

declare interface IWeaponSlots {
  id: number;
  ammo: number;
}

declare interface IPlayer {

  name: string;
  id: number;
  position: Vector3;
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
}

declare interface IVehicle {

  health: number;
  position: Vector3;
  driver: IPlayer;
  plate: string;
  model: number;

  repair(): void;
  putPlayer(player: IPlayer, seat: number): void;
}