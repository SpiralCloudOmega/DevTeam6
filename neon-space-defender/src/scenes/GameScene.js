import { Player, Bullet } from '../entities/Player';
import { Enemy } from '../entities/Enemy';

export class GameScene extends Phaser.Scene {
    constructor() {
        super({ key: 'GameScene' });
    }

    create() {
        // Starfield background
        this.add.particles(0, 0, 'bullet_neon', {
            x: { min: 0, max: 800 },
            y: { min: 0, max: 600 },
            quantity: 2,
            frequency: 100,
            lifespan: 4000,
            speedY: { min: 50, max: 150 },
            scale: { min: 0.1, max: 0.5 },
            alpha: { start: 1, end: 0 },
            emitting: true
        });

        // Player
        this.player = new Player(this, 400, 500);

        // Bullets Group
        this.bullets = this.physics.add.group({
            classType: Bullet,
            maxSize: 30,
            runChildUpdate: true
        });

        // Enemies Group
        this.enemies = this.physics.add.group({
            classType: Enemy,
            maxSize: 20,
            runChildUpdate: true
        });

        // Collision
        this.physics.add.overlap(this.bullets, this.enemies, this.hitEnemy, null, this);
        this.physics.add.overlap(this.player, this.enemies, this.hitPlayer, null, this);

        // Spawn Loop
        this.time.addEvent({
            delay: 1000,
            callback: this.spawnEnemy,
            callbackScope: this,
            loop: true
        });

        // Score
        this.score = 0;
        this.scoreText = this.add.text(16, 16, 'SCORE: 0', { 
            fontSize: '32px', 
            fill: '#fff',
            fontFamily: '"Courier New", Courier, monospace'
        }).setShadow(0, 0, 10, '#00ff00', 0);
    }

    update(time, delta) {
        this.player.update(time);
    }

    spawnEnemy() {
        const enemy = this.enemies.get();
        if (enemy) {
            enemy.spawn();
        }
    }

    hitEnemy(bullet, enemy) {
        bullet.setActive(false);
        bullet.setVisible(false);
        
        enemy.setActive(false);
        enemy.setVisible(false);

        // Particles
        const particles = this.add.particles(enemy.x, enemy.y, 'bullet_neon', {
            speed: { min: 50, max: 200 },
            angle: { min: 0, max: 360 },
            scale: { start: 1, end: 0 },
            alpha: { start: 1, end: 0 },
            lifespan: 500,
            quantity: 10,
            blendMode: 'ADD'
        });
        
        // Auto destroy particles
        this.time.delayedCall(500, () => particles.destroy());

        this.score += 100;
        this.scoreText.setText('SCORE: ' + this.score);
        
        // Screen Shake
        this.cameras.main.shake(100, 0.01);
    }

    hitPlayer(player, enemy) {
        this.cameras.main.shake(500, 0.05);
        this.physics.pause();
        this.add.text(400, 300, 'GAME OVER', { 
            fontSize: '64px', 
            fill: '#ff0000',
            fontFamily: '"Courier New", Courier, monospace'
        }).setOrigin(0.5).setShadow(0, 0, 20, '#ff0000', 4);
        
        this.add.text(400, 400, 'Click to Restart', { 
            fontSize: '24px', 
            fill: '#fff' 
        }).setOrigin(0.5);

        this.input.on('pointerdown', () => {
            this.scene.restart();
        });
    }
}
