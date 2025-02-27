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


// ========================================= pickups =========================================

/**
 * Get a pickup by id
 * @param id 
 */
declare function getPickup(id: number): IPickup;

/**
 * Create a pickup
 * @param model
 * @param pickupType
 * @param position
 * @param virtualworld
 * @param isStatic
 */
declare function createPickup(model: number, pickupType: number, position: Vector3, virtualworld: number, isStatic: boolean): IPickup;

// ========================================= menus =========================================

/**
 * Get a menu by id
 * @param id 
 */
declare function getMenu(id: number): IMenu;

/**
 * Create a menu
 * @param title
 * @param position
 * @param columns
 * @param col1Width
 * @param col2Width
 */
declare function createMenu(title: string, position: Vector2, columns: number, col1Width: number, col2Width: number): IMenu;

// ========================================= textdraws =========================================

/**
 * Get a textdraw by id
 * @param id
 */
declare function getTextDraw(id: number, player?: IPlayer): ITextDraw | IPlayerTextDraw;

/**
 * Create a textdraw
 * @param position
 * @param text
 * @param player optional
 */
declare function createTextDraw(position: Vector2, text: string, player?: IPlayer): ITextDraw | IPlayerTextDraw;

// ========================================= objects =========================================

/**
 * Get an object by id
 * @param id
 */
declare function getObject(id: number, player?: IPlayer): IObject | IPlayerObject;

/**
 * Create an object
 * @param model
 * @param position
 * @param rotation
 * @param drawDistance optional
 * @param player optional
 */
declare function createObject(model: number, position: Vector3, rotation: Quat, drawDistance?: number, player?: IPlayer): IObject | IPlayerObject;
