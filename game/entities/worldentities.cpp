void GeometryTile::on_draw(GeometryRenderer& painter) {
	painter.drawString(tile->pos, sprite, "geometry");
}
