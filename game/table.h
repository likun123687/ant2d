namespace game {
enum TableName { kSprite };

class Table {
public:
    virtual TableName GetTableName() = 0;
};

}
