options gen2

require node
require grngame_renderer

class ParticleEmitterNode : Node {
    emit : ParticleEmitter

    def ParticleEmitterNode(name : string) {
        Node`Node(self, name)
        self.emit = particle_emitter_create()
    }

    def set_lifetime(v : float) {
        emit.lifetime = v
    }

    def set_explosiveness(v : float) {
        emit.explosiveness = v
    }

    def set_one_shot(v : bool) {
        emit.one_shot = v
    }

    def set_local_coords(v : bool) {
        emit.local_coords = v
    }

    def set_direction(v : float) {
        emit.direction = v
    }

    def set_spread(v : float) {
        emit.spread = v
    }

    def set_initial_velocity(v : float) {
        emit.initial_velocity = v
    }

    def set_gravity(v : float) {
        emit.gravity = v
    }

    def set_damping(v : float) {
        emit.damping = v
    }

    def set_scale_start(v : float) {
        emit.scale_start = v
    }

    def set_scale_end(v : float) {
        emit.scale_end = v
    }

    def set_scale_amount_curve(v : float) {
        emit.scale_amount_curve = v
    }

    def set_amount(v : int16) {
        emit.amount = v
    }

    def set_color_start(r, g, b, a : uint8) {
        emit.red_start   = r
        emit.green_start = g
        emit.blue_start  = b
        emit.alpha_start = a
    }

    def set_color_end(r, g, b, a : uint8) {
        emit.red_end   = r
        emit.green_end = g
        emit.blue_end  = b
        emit.alpha_end = a
    }

    def set_emit_accumulator(v : float) {
        emit.emit_accumulator = v
    }

    def set_emit_cycle_time(v : float) {
        emit.emit_cycle_time = v
    }

    def set_one_shot_done(v : bool) {
        emit.one_shot_done = v
    }

    def override update(dt : float) {
        super.update(dt)
        let wp = get_world_position()
        emit.position.x = wp.x
        emit.position.y = wp.y
        emitter_update(emit, dt)
    }

    def override render() {
        super.render()
        emitter_render(emit)
    }

    def override destroy() {
        super.destroy()
        particle_emitter_destroy(emit)
    }
}