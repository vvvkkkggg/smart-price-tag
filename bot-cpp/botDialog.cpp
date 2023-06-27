#include "botDialog.h"
#include "botParser.h"
#include "botConsts.h"
#include "botTools.h"
#include "botDB.h"
#include "botKeyboards.h"

TgBot::InlineKeyboardMarkup::Ptr inlineConfirmationKeyboard = BotKeyboard::getInlineKeyboard(
        BotConst::inlineButtonsForSetConfirmation,
        CallbackParser::dumpInlineServerDataCallbackButton
);

void userRegistrationDialog(TgBot::Bot &bot, sw::redis::Redis &redis, const TgBot::Message::Ptr &message) {
    BotTool::sendMessage(bot, message->chat->id, "\"Умный ценник\" приветствует Вас!\n\n"
                                                 "Для полноценной работы с Вашим сервером мне понадобятся некоторые данные."
                                                 " Сперва отправьте URL вашего сервера.");
    BotDB::updateUserNode(
            redis,
            message->from->id,
            BotDB::createUser(
                    redis,
                    message->from->id,
                    BotStructure::createUser(
                            BotConst::EMPTY_DATA,
                            BotConst::EMPTY_DATA,
                            UserStates::CREATE_USER
                    )
            ), UserStates::WAIT_HOST);
}

void userServerDialog(TgBot::Bot &bot, sw::redis::Redis &redis, const TgBot::Message::Ptr &message, int userId,
                      struct User *user) {

    if (user->node == UserStates::WAIT_HOST) {
        std::string text = "Сохранили URL сервера - _%s_."
                           " Теперь нужен секретный ключ, пришлите его!\n\n"
                           "Если ошиблись, то напишите `/drop`, а затем пришлите верную ссылку на сервер.";

        std::ostringstream formatResult;
        formatResult << boost::format(text) % message->text;

        BotTool::sendMessage(bot, userId, formatResult.str());
        BotDB::updateUserHost(redis, userId, user, message->text);
        BotDB::updateUserNode(redis, userId, user, UserStates::WAIT_SECRET_KEY);

        return;
    }

    if (user->node == UserStates::WAIT_SECRET_KEY) {
        user = BotDB::updateUserSecretKey(redis, userId, user, message->text);

        std::string text = "Сохранили ключ. Давай сверим, все ли верно было введено? Итак, твои данные:\n\n"
                           "*Сервер* - _%s_\n"
                           "*Секретный ключ* - _%s_\n\n"
                           "Если ошиблись, то нажмите `нет`, либо введите `/drop`.";

        std::ostringstream formatResult;
        formatResult << boost::format(text) % user->host % user->secretKey;

        BotTool::sendMessage(bot, userId, formatResult.str(), inlineConfirmationKeyboard);
        BotDB::updateUserNode(redis, userId, user, UserStates::WAIT_CONFIRMATION_SERVER_DATA);

        return;
    }

    BotTool::sendMessage(bot, userId,
                         "Требуется либо подтвердить, либо опровергнуть введенные данные!");
}

bool isWaitUserServerData(int node) {
    return node == UserStates::WAIT_HOST || node == UserStates::WAIT_SECRET_KEY ||
           node == UserStates::WAIT_CONFIRMATION_SERVER_DATA;
}

bool
BotDialog::dialogChainResponsibility(TgBot::Bot &bot, sw::redis::Redis &redis, const TgBot::Message::Ptr &message) {
    int userId = message->from->id;
    struct User *user = BotDB::getUser(redis, userId);

    if (!user) {
        userRegistrationDialog(bot, redis, message);
        return true;
    }

    if (isWaitUserServerData(user->node)) {
        userServerDialog(bot, redis, message, userId, user);
        return true;
    }

    return false;
}
