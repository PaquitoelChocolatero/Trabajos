package es.uc3m.g1.musey.model.api.lastfm

import com.google.gson.JsonDeserializationContext
import com.google.gson.JsonDeserializer
import com.google.gson.JsonElement
import com.google.gson.annotations.JsonAdapter
import java.lang.reflect.Type

@JsonAdapter(Artist.Deserializer::class)
data class Artist(
    var name: String
) {
    class Deserializer : JsonDeserializer<Artist> {
        override fun deserialize(
            json: JsonElement?,
            typeOfT: Type?,
            context: JsonDeserializationContext?
        ): Artist {
            var name: String = ""

            if (json?.isJsonObject == true) {
                name = json.asJsonObject?.get("name")?.asString ?: ""
            } else if (json?.isJsonObject == false) {
                name = json.asString ?: ""
            }

            return Artist(name)
        }
    }
}