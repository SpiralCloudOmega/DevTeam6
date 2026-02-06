import Phaser from 'phaser';
import { GameScene } from './scenes/GameScene';
import '../style.css';

const config = {
    type: Phaser.AUTO,
    width: 800,
    height: 600,
    parent: 'app',
    physics: {
        default: 'arcade',
        arcade: {
            gravity: { y: 0 },
            debug: false
        }
    },
    scene: [GameScene],
    backgroundColor: '#000000'
};

new Phaser.Game(config);
