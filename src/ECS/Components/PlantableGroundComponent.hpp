#pragma once

struct PlantableGroundComponent
{
    unsigned short stageOfGrowth = 0;
};

void DecodePlantableGroundComponent(YAML::Node &_n, Canis::Entity &_entity, Canis::SceneManager *_sceneManager)
{
if (auto plantableGroundComponent = _n["PlantableGroundComponent"])
    {
        auto &pgc = _entity.AddComponent<PlantableGroundComponent>();
        pgc.stageOfGrowth = plantableGroundComponent["stageOfGrowth"].as<unsigned short>();
    }
}