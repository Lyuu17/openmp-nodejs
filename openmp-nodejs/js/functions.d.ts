/// <reference path="./types.d.ts"/>

/**
 * Print a message using open.mp
 * @param message 
 */
declare function print(message: string): void;

/**
 * Print a message using open.mp
 * @param message 
 */
declare function printError(message: string): void;

/**
 * Print a message using open.mp
 * @param message 
 */
declare function printWarning(message: string): void;

/**
 * Listen to a specific event
 * @param eventName 
 * @param handler 
 */
declare function on(eventName: string, handler: IEventHandler): void;

/**
 * Emit an event
 * @param eventName
 * @param args
 */
declare function emit(eventName: string, ...args: any): void;

/**
 * Send a client message globally
 * @param message 
 * @param colour RGBA value. Default 0xFFFFFFFF
 */
declare function message(message: string, colour?: number): void;

/**
 * Send a client message to a specific player
 * @param message 
 * @param player
 * @param colour RGBA value. Default 0xFFFFFFFF
 */
declare function messagePlayer(message: string, player: IPlayer, colour?: number): void;

// ========================================= player =========================================

/**
 * Get a player by id
 * @param id 
 */
declare function getPlayer(id: number): IPlayer;

// ========================================= vehicle =========================================

/**
 * Get a vehicle by id
 * @param id 
 */
declare function getVehicle(id: number): IVehicle;

/**
 * Create a vehicle
 * @param isStatic 
 * @param model 
 * @param position 
 * @param angle Default: 0
 * @param color1 Default: -1 (random)
 * @param color2 Default: -1 (random)
 * @param respawnDelay Default: 300
 * @param addSiren Default: false
 */
declare function createVehicle(isStatic: boolean, model: number, position: Vector3, angle?: number, color1?: number, color2?: number, respawnDelay?: number, addSiren?: boolean): IVehicle;

// ========================================= classes =========================================

/**
 * Add a player class
 * @param model
 * @param team
 * @param position
 * @param angle
 * @param weapons
 */
declare function addPlayerClass(model: number, team: number, position: Vector3, angle: number, weapons?: IWeaponSlots[]): void;
