export class Player extends Phaser.Physics.Arcade.Sprite {
    constructor(scene, x, y) {
        // Create a procedural neon texture
        const graphics = scene.make.graphics({ x: 0, y: 0, add: false });
        
        // Glow effect
        graphics.lineStyle(4, 0x00ff00, 0.5);
        graphics.strokeTriangle(0, -20, 16, 16, -16, 16);
        
        // Core lines
        graphics.lineStyle(2, 0xccffcc, 1);
        graphics.strokeTriangle(0, -20, 16, 16, -16, 16);
        
        // Generate texture from graphics
        graphics.generateTexture('player_neon', 40, 40);

        super(scene, x, y, 'player_neon');
        
        scene.add.existing(this);
        scene.physics.add.existing(this);
        
        this.setCollideWorldBounds(true);
        this.setDrag(100);
        this.setMaxVelocity(400);
        
        this.cursors = scene.input.keyboard.createCursorKeys();
        this.wasd = scene.input.keyboard.addKeys('W,S,A,D');
        this.space = scene.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.SPACE);
        
        this.lastFired = 0;
        
        // Add additive blend mode for glow
        this.setBlendMode(Phaser.BlendModes.ADD);
    }

    update(time) {
        this.handleMovement();
        this.handleShooting(time);
    }

    handleMovement() {
        this.setAcceleration(0);

        if (this.cursors.left.isDown || this.wasd.A.isDown) {
            this.setAccelerationX(-800);
        } else if (this.cursors.right.isDown || this.wasd.D.isDown) {
            this.setAccelerationX(800);
        }

        if (this.cursors.up.isDown || this.wasd.W.isDown) {
            this.setAccelerationY(-800);
        } else if (this.cursors.down.isDown || this.wasd.S.isDown) {
            this.setAccelerationY(800);
        }
        
        // Bank-turn visual effect
        this.setRotation(this.body.velocity.x * 0.001);
    }

    handleShooting(time) {
        if (this.space.isDown && time > this.lastFired) {
            this.fireBullet();
            this.lastFired = time + 150; // Fire rate
        }
    }

    fireBullet() {
        const bullet = this.scene.bullets.get(this.x, this.y - 20);
        if (bullet) {
            bullet.fire(this.x, this.y - 20);
        }
    }
}

export class Bullet extends Phaser.Physics.Arcade.Sprite {
    constructor(scene, x, y) {
        // Create procedural bullet texture if not exists
        if (!scene.textures.exists('bullet_neon')) {
            const graphics = scene.make.graphics({ x: 0, y: 0, add: false });
            graphics.fillStyle(0x00ffff, 1);
            graphics.fillCircle(4, 4, 4);
            graphics.generateTexture('bullet_neon', 8, 8);
        }

        super(scene, x, y, 'bullet_neon');
        this.setBlendMode(Phaser.BlendModes.ADD);
    }

    fire(x, y) {
        this.body.reset(x, y);
        this.setActive(true);
        this.setVisible(true);
        this.setVelocityY(-600);
    }

    update(time, delta) {
        if (this.y < -50) {
            this.setActive(false);
            this.setVisible(false);
        }
    }
}
