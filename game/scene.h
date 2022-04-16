namespace game
class Scene {
public:
    virtual void OnEnter(Game *g) = 0;
    virtual void Update(float dt)  = 0;
    virtual void OnExit() = 0;
    virtual void GetOnLoadCallback() = 0;
    virtual void SetUnLoadCallback() = 0;
};

class SceneManager {
public:
    void Load(Scene sn);
    void UnLoad(Scene sn);
    void Setup(Game *g);
    void Update(float dt);
    void SetDefault(Scene sn);
    void Push(Scene sn);
    Scene Pop();
    Scene Peek();
    void Clear();

private:
    Game *g_;
    std::vector<std::unique_ptr<Scene>> stack_;
    Scene *hScene_;
};
}
