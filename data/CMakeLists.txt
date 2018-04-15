# Background
install(FILES bg.png DESTINATION data)

# Ants
install(FILES schema_ant.json DESTINATION data/entity)
install(FILES ant_default.json DESTINATION data/entity)
install(FILES ant_soldier.json DESTINATION data/entity)
install(FILES ant_worker.json DESTINATION data/entity)
install(FILES ant_explorer.json DESTINATION data/entity)

# Events
install(FILES event_sample.json DESTINATION data/events)
install(FILES schema_events.json DESTINATION data/events)
