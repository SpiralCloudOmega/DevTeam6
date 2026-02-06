export class Enemy extends Phaser.Physics.Arcade.Sprite {
    constructor(scene, x, y) {
        // Create procedural enemy texture if not exists
        if (!scene.textures.exists('enemy_neon')) {
            const graphics = scene.make.graphics({ x: 0, y: 0, add: false });
            graphics.lineStyle(2, 0xff00ff, 1);
            graphics.strokeRect(0, 0, 32, 32);
            graphics.lineStyle(1, 0xff00ff, 0.5);
            graphics.beginPath();
            graphics.moveTo(0,0); graphics.lineTo(32,32);
            graphics.moveTo(32,0); graphics.lineTo(0,32);
            graphics.strokePath();
            graphics.generateTexture('enemy_neon', 32, 32);
        }

        super(scene, x, y, 'enemy_neon');
        this.setBlendMode(Phaser.BlendModes.ADD);
    }

    spawn() {
        // Random spawn at top
        const x = Phaser.Math.Between(50, 750);
        this.body.reset(x, -50);
        this.setActive(true);
        this.setVisible(true);
        this.setVelocityY(Phaser.Math.Between(100, 300));
        this.setAngularVelocity(Phaser.Math.Between(-100, 100));
    }

    update(time, delta) {
        if (this.y > 650) {
            this.setActive(false);
            this.setVisible(false);
        }
    }
}
